//----------------------------------------------------------------------------
//
//  Workfile: mySqlTest.cpp
//
//  Copyright: Jim Wright 2024
//
//  Notes:
//     This is the test of the mySql Class
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <iomanip>
#include <signal.h>
#include "loggerSingleton.h"
#include "stopWatch.h"
#include "MQTTAsync.h"
#include "math.h"
#include "connection.h"
#include "mqtt.h"

using namespace std::chrono_literals;
//----------------------------------------------------------------------------
//  Global and Static data
//----------------------------------------------------------------------------
bool gVerboseMQTT = true;
std::shared_ptr<MQTT> gMqtt = std::shared_ptr<MQTT>(new MQTT());
bool gRunTheThread = true;

//----------------------------------------------------------------------------
//  Purpose:
//   Run the airline for one second
//
//  Notes:
//
//----------------------------------------------------------------------------
void stopThread(int s)
{
  LoggerSingleton::getInstance()->writeWarning("Stopping app");
  gRunTheThread = false;
}

//----------------------------------------------------------------------------
//  Purpose:
//   The call back function
//
//  Notes:
//
//----------------------------------------------------------------------------
void messageArrived(char *topicName, int topicLen, MQTTAsync_message *message)
{
  std::string payload((char*)message->payload);
  payload = payload.substr(0,message->payloadlen);
  std::string topic(topicName);
  LoggerSingleton::getInstance()->writeInfo("User Message arived:" + std::string(topicName) + 
    " len:" + std::to_string(topicLen) + " plen:"+ std::to_string(message->payloadlen) + " message:" + std::string(payload));
}


//----------------------------------------------------------------------------
//  Purpose:
//   Entry point
//
//  Notes:
//
//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
  signal(SIGTERM, stopThread);
  signal(SIGINT, stopThread);

  int rc=0;

  LoggerSingleton::getInstance()->setFilename("mqttSinTest",true);
  LoggerSingleton::getInstance()->setSeverity(SEVERITY_INFO);

  if(argc != 2)
  {
    LoggerSingleton::getInstance()->writeError("Usage: mqttSinTest <mqtt connection file> ");
    return -1;
  }

  int count = 0;

  gMqtt->loadConfiguration(argv[1]);
  gMqtt->openMQTT();
  gMqtt->subscribe((char*)"test/health");
  gMqtt->setMessageArrivedFunction(&messageArrived);  
  gMqtt->setAutoConnectOnSend(true);

  std::shared_ptr<StopWatch> sendWatch = 
      std::shared_ptr<StopWatch>(new StopWatch(static_cast<uint64_t>(15000)));
  sendWatch->reset();
  
  while(true == gRunTheThread)
  {
    if(true == sendWatch->isExpired())
    {
      std::string syncTime = "{ \"cmd\": \"mqttping\", \"msg\": \""+std::to_string(count)+"\"}";
      LoggerSingleton::getInstance()->writeInfo(syncTime);
      gMqtt->send("test/health",syncTime);
      count++;
      sendWatch->reset();
    }

    std::this_thread::sleep_for(100ms);
  }

  return EXIT_SUCCESS;

}
