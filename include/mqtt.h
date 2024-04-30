//----------------------------------------------------------------------------
//
//  $Workfile: randomSingleton.h$
//
//  $Revision: X$
//
//  Modification History:
//  $Log:
//  $
//
//  Notes:
//     This is a singleton for dealing with a random number that is predictable
//
//----------------------------------------------------------------------------
#ifndef MQTT_H
#define MQTT_H

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include <cstdlib> 
#include <ctime> 
#include <iostream>
#include "MQTTAsync.h"
#include "connection.h"

const int QOS = 1;
const int TIMEOUT = 10000L;

//----------------------------------------------------------------------------
//  Class Declarations
//----------------------------------------------------------------------------
//
// Class Name: RandomSingleton
// 
// Purpose:
//      The RandomSingleton
//
//----------------------------------------------------------------------------
class MQTT
{
  public:
    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
    static MQTT &GetInstance();
    ~MQTT();
    void connect(Connection connect);
    void send(char* topic, char* payload, int payloadLength);
    static void onConnect(void* context, MQTTAsync_successData* response);
    static void onConnectFailure(void* context, MQTTAsync_failureData* response);
    static void onSend(void* context, MQTTAsync_successData* response);
    static void onDisconnect(void* context, MQTTAsync_successData* response);
    static void connlost(void *context, char *cause);

  private:
    //----------------------------------------------------------------------------
    //  Class Atributes
    //----------------------------------------------------------------------------
    static std::shared_ptr<MQTT> sInstance;
    MQTTAsync mClient;
    volatile MQTTAsync_token mDeliveredtoken;
//    static int mFinished;

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
    MQTT();
};
#endif
