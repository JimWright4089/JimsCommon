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
#include "loggerSingleton.h"
#include "MQTTAsync.h"
#include "mqtt.h"
#include "stdio.h"
#include "string.h"

using namespace std::chrono_literals;

//----------------------------------------------------------------------------
//  Global and Static data
//----------------------------------------------------------------------------
void (*MQTT::mMessageArrivedFunction)(char *topicName, int topicLen, MQTTAsync_message *message) = NULL;
std::shared_ptr<std::queue<std::shared_ptr<MQTTMessage>>>MQTT::mMesageQueue = std::shared_ptr<std::queue<std::shared_ptr<MQTTMessage>>>(new std::queue<std::shared_ptr<MQTTMessage>>);
int MQTT::mVerboseCount = 0;

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

  std::string user = mConnection.getUser();
  std::string password = mConnection.getPassword();

  MQTTAsync_create(&mClient,
                mConnection.getMQTTAddress().c_str(),
                mConnection.getClientID().c_str(),
                MQTTCLIENT_PERSISTENCE_NONE, NULL);

  MQTTAsync_setCallbacks(mClient, NULL, connlost, messageArrived, NULL);
  conn_opts.keepAliveInterval = 20;
  conn_opts.cleansession = 1;
  conn_opts.onSuccess = onConnect;
  conn_opts.onFailure = onConnectFailure;
  conn_opts.context = mClient;
  conn_opts.username = user.c_str();
  conn_opts.password = password.c_str();

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
void MQTT::send(char* topic, char* payload, int payloadLength)
{
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
//   subscribe to a topic
//
//  Notes:
//
//----------------------------------------------------------------------------
void MQTT::subscribe(char* topic)
{
  int rc;
  LoggerSingleton::getInstance()->writeInfo("Subscribing to topic:" + std::string(topic));

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

  std::shared_ptr<MQTTMessage> mqttMessage = std::shared_ptr<MQTTMessage>(new MQTTMessage(topicName, (char*)message->payload));
  mMesageQueue->push(mqttMessage);  

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
  MQTTAsync client = (MQTTAsync)context;
  MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
  int rc;
  LoggerSingleton::getInstance()->writeError("Connection lost cause:" + std::string(cause) + "Reconnecting");
  conn_opts.keepAliveInterval = 20;
  conn_opts.cleansession = 1;
  if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
  {
    LoggerSingleton::getInstance()->writeError("Failed to start connect, return code " + std::to_string(rc));
  }
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
  LoggerSingleton::getInstance()->writeError("Connect failed, rc:" + (response ? std::to_string(response->code) : "0"));
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
