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
#include <signal.h>
#include "loggerSingleton.h"
#include "MQTTAsync.h"
#include "math.h"
#include "connection.h"
#include "mqtt.h"

//----------------------------------------------------------------------------
//  Defines
//----------------------------------------------------------------------------
#define TOPIC    (char*)"sin/#"
#define QOS         1
#define TIMEOUT     10000L

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

  LoggerSingleton::getInstance()->setFilename("mqttSinTest",true);
  LoggerSingleton::getInstance()->setSeverity(SEVERITY_INFO);

  if(argc != 2)
  {
    LoggerSingleton::getInstance()->writeError("Usage: mqttSinTest <mqtt connection file> ");
    return -1;
  }

  gMqtt->loadConfiguration(argv[1]);
  gMqtt->openMQTT();
  gMqtt->subscribe(TOPIC);

  std::shared_ptr<std::queue<std::shared_ptr<MQTTMessage>>> theQueue = gMqtt->getMessageQueue();

  while(true == gRunTheThread)
  {
    if(!theQueue->empty())
    {
      std::shared_ptr<MQTTMessage> message = theQueue->front();
      theQueue->pop();
      LoggerSingleton::getInstance()->writeInfo(message->getTopic() + " " + message->getPayload());
    }

    usleep(100000L);
  }

  return rc;
}
