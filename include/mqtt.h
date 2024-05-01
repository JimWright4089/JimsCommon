//----------------------------------------------------------------------------
//
//  Workfile: mqtt.h$
//
//  Copyright: Jim Wright 2024
//
//  Notes:
//     This is a singleton publishing and subscribing to mqtt topics
//
//----------------------------------------------------------------------------
#ifndef MQTT_H
#define MQTT_H

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include "MQTTAsync.h"
#include "connection.h"

const int QOS = 1;
const int TIMEOUT = 10000L;

//----------------------------------------------------------------------------
//  Class Declarations
//----------------------------------------------------------------------------
//
// Class Name: MQTT
// 
// Purpose:
//      
//
//----------------------------------------------------------------------------
class MQTT
{
  public:
    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
    static MQTT &getInstance();
    void connect(Connection connect);
    void send(char* topic, char* payload, int payloadLength);
    static void onConnect(void* context, MQTTAsync_successData* response);
    static void onConnectFailure(void* context, MQTTAsync_failureData* response);
    static void onSend(void* context, MQTTAsync_successData* response);
    static void onDisconnect(void* context, MQTTAsync_successData* response);
    static void connlost(void *context, char *cause);
    ~MQTT();
    
  private:
    //----------------------------------------------------------------------------
    //  Class Atributes
    //----------------------------------------------------------------------------
    static std::shared_ptr<MQTT> sInstance;
    MQTTAsync mClient;
    volatile MQTTAsync_token mDeliveredtoken;

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
    MQTT();
};
#endif
