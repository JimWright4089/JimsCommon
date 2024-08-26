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
#include <chrono>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "loggerSingleton.h"
#include "MQTTAsync.h"
#include "math.h"
#include "connection.h"
#include "mqtt.h"

using namespace std::chrono_literals;
//----------------------------------------------------------------------------
//  Defines
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Global and Static data
//----------------------------------------------------------------------------
bool gVerboseMQTT = true;
bool gRunTheThread = true;
std::shared_ptr<MQTT> gMqtt = std::shared_ptr<MQTT>(new MQTT());

//----------------------------------------------------------------------------
//  Purpose:
//   Stop all of the threads
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
//   Entry point
//
//  Notes:
//
//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
  int rc=0;

  signal(SIGTERM, stopThread);
  signal(SIGINT, stopThread);

  LoggerSingleton::getInstance()->setFilename("mqttSubTest",true);
  LoggerSingleton::getInstance()->setSeverity(SEVERITY_INFO);

  if(argc != 2)
  {
    LoggerSingleton::getInstance()->writeError("Usage: mqttSinTest <mqtt connection file> ");
    return -1;
  }

  gMqtt->loadConfiguration(argv[1]);
  gMqtt->openMQTT();
  std::this_thread::sleep_for(1s);
  gMqtt->subscribe((char*)"test/health");

  std::shared_ptr<std::queue<std::shared_ptr<MQTTMessage>>> theQueue = gMqtt->getMessageQueue();

  while(true == gRunTheThread)
  {
    if(false == gMqtt->isConnected())
    {
      LoggerSingleton::getInstance()->writeInfo("connect to MQTT");
      gMqtt->openMQTT();
      std::this_thread::sleep_for(1s);
      if(true == gMqtt->isConnected())
      {
        gMqtt->reSubscribe();
      }
    }

    if(!theQueue->empty())
    {
      std::shared_ptr<MQTTMessage> message = theQueue->front();
      theQueue->pop();
      LoggerSingleton::getInstance()->writeInfo(message->getTopic() + " " + message->getPayload());
    }

    std::this_thread::sleep_for(500ms);
  }

  return rc;
}
