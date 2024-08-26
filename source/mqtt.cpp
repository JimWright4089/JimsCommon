//----------------------------------------------------------------------------
//
//  Workfile: mqtt.cpp
//
//  Copyright: Jim Wright 2024
//
//  Notes:
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <cstdlib> 
#include <ctime> 
#include <thread>
#include <vector>
#include "loggerSingleton.h"
#include "randomSingleton.h"
#include "MQTTAsync.h"
#include "mqtt.h"
#include "stdio.h"
#include "string.h"

using namespace std::chrono_literals;

//----------------------------------------------------------------------------
//  Global and Static data
//----------------------------------------------------------------------------
void (*MQTT::mMessageArrivedFunction)(char *topicName, int topicLen, MQTTAsync_message *message) = NULL;
std::shared_ptr<std::queue<std::shared_ptr<MQTTMessage>>>MQTT::mMesageQueue = 
    std::shared_ptr<std::queue<std::shared_ptr<MQTTMessage>>>(new std::queue<std::shared_ptr<MQTTMessage>>);
int MQTT::mVerboseCount = 0;
bool MQTT::mUseQueue = false;
bool MQTT::mConnected = false;
bool MQTT::mAutoConnectOnSend = false;
int MQTT::mConnectFailedLog = 0;
std::shared_ptr<std::vector<std::shared_ptr<std::string>>> MQTT::mSubscribeList = 
  std::shared_ptr<std::vector<std::shared_ptr<std::string>>>(new std::vector<std::shared_ptr<std::string>>);

//----------------------------------------------------------------------------
//  Purpose:
//   Constructor
//
//  Notes:
//
//----------------------------------------------------------------------------
MQTT::MQTT()
{
}

//----------------------------------------------------------------------------
//  Purpose:
//   Destructor
//
//  Notes:
//
//----------------------------------------------------------------------------
MQTT::~MQTT()
{
  MQTTAsync_destroy(&mClient);
}

//----------------------------------------------------------------------------
//  Purpose:
//   load configuration from file
//
//  Notes:
//
//----------------------------------------------------------------------------
void MQTT::loadConfiguration(std::string fileName)
{
  std::string sqlFilename(fileName);
  if(false == mConnection.loadFromFile(sqlFilename))
  {
    LoggerSingleton::getInstance()->writeError("MQTT: The connection is blank\n");
  }

  mConnection.print();

  mUser = mConnection.getUser();
  mPassword = mConnection.getPassword();
  mMQttAddress = mConnection.getMQTTAddress();
  mClientID = mConnection.getClientID();
}

//----------------------------------------------------------------------------
//  Purpose:
//   load configuration from file
//
//  Notes:
//
//----------------------------------------------------------------------------
void MQTT::loadConfiguration(std::string address, std::string  port, std::string user, std::string password)
{
  mUser = user;
  mPassword = password;
  mMQttAddress = std::string("tcp://")+address+std::string(":")+port;
  int randomNumber = RandomSingleton::getInstance().next();
  std::string randomString = std::to_string(randomNumber);
  mClientID = "TimeSync"+randomString;
}

//----------------------------------------------------------------------------
//  Purpose:
//   connect to a server
//
//  Notes:
//
//----------------------------------------------------------------------------
void MQTT::openMQTT()
{
  int rc=0;
  MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
  MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
  MQTTAsync_token token;

  MQTTAsync_create(&mClient,
                mMQttAddress.c_str(),
                mClientID.c_str(),
                MQTTCLIENT_PERSISTENCE_NONE, NULL);

  MQTTAsync_setCallbacks(mClient, NULL, connlost, messageArrived, NULL);
  conn_opts.keepAliveInterval = 20;
  conn_opts.cleansession = 1;
  conn_opts.onSuccess = onConnect;
  conn_opts.onFailure = onConnectFailure;
  conn_opts.context = mClient;
  conn_opts.username = mUser.c_str();
  conn_opts.password = mPassword.c_str();

  if ((rc = MQTTAsync_connect(mClient, &conn_opts)) != MQTTASYNC_SUCCESS)
  {
    LoggerSingleton::getInstance()->writeError("Failed to start connect, return code:" + std::to_string(rc));
  }

  std::this_thread::sleep_for(2000ms);
}

//----------------------------------------------------------------------------
//  Purpose:
//   Send a text packet
//
//  Notes:
//
//----------------------------------------------------------------------------
void MQTT::send(const char* topic, const char* payload, int payloadLength)
{
  if(false == mConnected)
  {
    if(true == mAutoConnectOnSend)
    {
      LoggerSingleton::getInstance()->writeInfo("connect to MQTT");
      openMQTT();
      std::this_thread::sleep_for(1s);

      if(true == mConnected)
      {
        reSubscribe();
      }
    }
    else
    {
      LoggerSingleton::getInstance()->writeWarning("Attempt to send when not connected");
      return;
    }
  }

  MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
  MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
  int rc;
  opts.onSuccess = onSend;
  opts.context = mClient;
  pubmsg.payload = (void*)payload;
  pubmsg.payloadlen = payloadLength;
  pubmsg.qos = QOS;
  pubmsg.retained = 0;
  mDeliveredtoken = 0;
  if ((rc = MQTTAsync_sendMessage(mClient, topic, &pubmsg, &opts)) != MQTTASYNC_SUCCESS)
  {
    LoggerSingleton::getInstance()->writeError("Failed to start sendMessage, return code:" + std::to_string(rc));
  }
}

//----------------------------------------------------------------------------
//  Purpose:
//   Send a text packet
//
//  Notes:
//
//----------------------------------------------------------------------------
void MQTT::send(const char* topic, std::string payload)
{
  send(topic,payload.c_str(),strlen(payload.c_str()));
}

//----------------------------------------------------------------------------
//  Purpose:
//   Send a text packet
//
//  Notes:
//
//----------------------------------------------------------------------------
void MQTT::send(std::string topic, std::string payload)
{
  send(topic.c_str(),payload.c_str(),strlen(payload.c_str()));
}

//----------------------------------------------------------------------------
//  Purpose:
//   subscribe to a topic
//
//  Notes:
//
//----------------------------------------------------------------------------
void MQTT::subscribe(char* topic, bool addTopic)
{
  int rc;
  LoggerSingleton::getInstance()->writeInfo("Subscribing to topic:" + std::string(topic));

  if(true == addTopic)
  {
    mSubscribeList->push_back(std::shared_ptr<std::string>(new std::string(topic)));
  }

  MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
  opts.onSuccess = onSubscribe;
  opts.onFailure = onSubscribeFailure;
  opts.context = mClient;
  if ((rc = MQTTAsync_subscribe(mClient, topic, QOS, &opts)) != MQTTASYNC_SUCCESS)
  {
    LoggerSingleton::getInstance()->writeError("Failed to start subscribe, return code:" + std::to_string(rc));
  }
}

//----------------------------------------------------------------------------
//  Purpose:
//   subscribe to a topic
//
//  Notes:
//
//----------------------------------------------------------------------------
void MQTT::subscribe(std::string topic, bool addTopic)
{
  subscribe((char*)topic.c_str(),addTopic);
}

//----------------------------------------------------------------------------
//  Purpose:
//   subscribe to a topic
//
//  Notes:
//
//----------------------------------------------------------------------------
void MQTT::subscribe(std::shared_ptr<std::string> topic, bool addTopic)
{
  subscribe(*topic, addTopic);
}

//----------------------------------------------------------------------------
//  Purpose:
//   Receive a message and send it to the listeners
//
//  Notes:
//
//----------------------------------------------------------------------------
int MQTT::messageArrived(void *context, char *topicName, int topicLen, MQTTAsync_message *message)
{
  if(0==(mVerboseCount%VERBOSE_MESSAGE_DISPLAY_MOD))
  {
    std::string payload((char*)message->payload);
    LoggerSingleton::getInstance()->writeDebug("Message arived:" + std::string(topicName) + " len:" + std::to_string(topicLen) + " message:" + std::string(payload));
  }
  mVerboseCount++;

  if(NULL != mMessageArrivedFunction)
  {
    mMessageArrivedFunction(topicName, topicLen, message);
  }

  if(true == mUseQueue)
  {
    std::shared_ptr<MQTTMessage> mqttMessage = std::shared_ptr<MQTTMessage>(
        new MQTTMessage(topicName, (char*)message->payload, message->payloadlen));
    mMesageQueue->push(mqttMessage);  
  }

  MQTTAsync_freeMessage(&message);
  MQTTAsync_free(topicName);

  return 1;
}

//----------------------------------------------------------------------------
//  Purpose:
//   Set the user function
//
//  Notes:
//
//----------------------------------------------------------------------------
void MQTT::setMessageArrivedFunction(void (*messageArrivedFunction)(char *topicName, int topicLen, MQTTAsync_message *message))
{
  mMessageArrivedFunction = messageArrivedFunction;
}

//----------------------------------------------------------------------------
//  Purpose:
//   Return the queue
//
//  Notes:
//
//----------------------------------------------------------------------------
std::shared_ptr<std::queue<std::shared_ptr<MQTTMessage>>> MQTT::getMessageQueue()
{
  mUseQueue = true;
  return mMesageQueue;
}

//----------------------------------------------------------------------------
//  Purpose:
//   connection lost call back
//
//  Notes:
//
//----------------------------------------------------------------------------
void MQTT::connlost(void *context, char *cause)
{
  if(NULL != cause)
  {
    LoggerSingleton::getInstance()->writeError("Connection lost cause:" + std::string(cause));
  }
  else
  {
    LoggerSingleton::getInstance()->writeError("Connection lost, null cause");
  }

  mConnected = false;
}

//----------------------------------------------------------------------------
//  Purpose:
//   disconnect call back
//
//  Notes:
//
//----------------------------------------------------------------------------
void MQTT::onDisconnect(void* context, MQTTAsync_successData* response)
{
  LoggerSingleton::getInstance()->writeDebug("Successful disconnection");
  mConnected = false;
}

//----------------------------------------------------------------------------
//  Purpose:
//   on send call back
//
//  Notes:
//
//----------------------------------------------------------------------------
void MQTT::onSend(void* context, MQTTAsync_successData* response)
{
  LoggerSingleton::getInstance()->writeDebug("Message with token value:" + std::to_string(response->token) + " delivery confirmed");
}

//----------------------------------------------------------------------------
//  Purpose:
//   connection failure call back
//
//  Notes:
//
//----------------------------------------------------------------------------
void MQTT::onConnectFailure(void* context, MQTTAsync_failureData* response)
{
  if(0==(mConnectFailedLog%100))
  {
    LoggerSingleton::getInstance()->writeError("Connect failed, rc:" + (response ? std::to_string(response->code) : "0")+" message count="+std::to_string(mConnectFailedLog));
  }
  mConnectFailedLog++;
  mConnected = false;
}

//----------------------------------------------------------------------------
//  Purpose:
//   on connect call back
//
//  Notes:
//
//----------------------------------------------------------------------------
void MQTT::onConnect(void* context, MQTTAsync_successData* response)
{
  LoggerSingleton::getInstance()->writeInfo("Successful MQTT connection");
  mConnectFailedLog = 0;
  mConnected = true;
}

//----------------------------------------------------------------------------
//  Purpose:
//   on subscribe
//
//  Notes:
//
//----------------------------------------------------------------------------
void MQTT::onSubscribe(void* context, MQTTAsync_successData* response)
{
  LoggerSingleton::getInstance()->writeDebug("Subscribe succeeded");
}

//----------------------------------------------------------------------------
//  Purpose:
//   on subscribe
//
//  Notes:
//
//----------------------------------------------------------------------------
void MQTT::onSubscribeFailure(void* context, MQTTAsync_failureData* response)
{
  LoggerSingleton::getInstance()->writeError("Subscribe failed, rc:" + (response ? std::to_string(response->code) : "0"));
}

//----------------------------------------------------------------------------
//  Purpose:
//   on subscribe
//
//  Notes:
//
//----------------------------------------------------------------------------
bool MQTT::isConnected()
{
  return mConnected;
}

//----------------------------------------------------------------------------
//  Purpose:
//   on subscribe
//
//  Notes:
//
//----------------------------------------------------------------------------
bool MQTT::getAutoConnectOnSend()
{
  return mAutoConnectOnSend;
}

//----------------------------------------------------------------------------
//  Purpose:
//   on subscribe
//
//  Notes:
//
//----------------------------------------------------------------------------
void MQTT::setAutoConnectOnSend(bool value)
{
  mAutoConnectOnSend = value;
}

//----------------------------------------------------------------------------
//  Purpose:
//   on subscribe
//
//  Notes:
//
//----------------------------------------------------------------------------
void MQTT::reSubscribe()
{
  for(int i=0;i<mSubscribeList->size();i++)
  {
    subscribe(mSubscribeList->at(i),false);
  }
 
}