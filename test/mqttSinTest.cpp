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
#include "loggerSingleton.h"
#include "MQTTAsync.h"
#include "math.h"
#include "connection.h"
#include "mqtt.h"

//----------------------------------------------------------------------------
//  Defines
//----------------------------------------------------------------------------
#define TOPIC1HZ    (char*)"sin/1hz/value"
#define TOPIC2HZ    (char*)"sin/2hz/value"
#define TOPIC5HZ   (char*)"sin/5hz/value"
#define QOS         1
#define TIMEOUT     10000L

//----------------------------------------------------------------------------
//  Global and Static data
//----------------------------------------------------------------------------
bool gVerboseMQTT = true;
std::shared_ptr<MQTT> gMqtt = std::shared_ptr<MQTT>(new MQTT());

//----------------------------------------------------------------------------
//  Purpose:
//   Send text to a topic
//
//  Notes:
//
//----------------------------------------------------------------------------
void send(char* topic, double value)
{
  char valueString[31];
  snprintf(valueString,30,"%f",value);
  gMqtt->send(topic,valueString,strlen(valueString));
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

  LoggerSingleton::getInstance()->setFilename("mqttSinTest",true);
  LoggerSingleton::getInstance()->setSeverity(SEVERITY_INFO);

  if(argc != 2)
  {
    LoggerSingleton::getInstance()->writeError("Usage: mqttSinTest <mqtt connection file> ");
    return -1;
  }

  gMqtt->loadConfiguration(argv[1]);
  gMqtt->openMQTT();

  int count = 0;
  double count1Hz = 0;
  double count2Hz = 0;
  double count5Hz = 0;
  double addAmount = 0.01;

  for(int i=0;i<30;i++)
  {
    count++;

    if(0==(count%2))
    {
      count5Hz += addAmount;
      send(TOPIC5HZ, sin(count5Hz));
      usleep(10000L);
    }

    if(5 == count)
    {
      count2Hz += addAmount;
      send(TOPIC2HZ, sin(count2Hz));
      usleep(10000L);
    }

    if(count >= 10)
    {
      count2Hz += addAmount;
      send(TOPIC2HZ, sin(count2Hz));
      usleep(10000L);
      count1Hz += addAmount;
      send(TOPIC1HZ, sin(count1Hz));
      count = 0;
    }

    usleep(100000L);
  }

  return rc;
}
