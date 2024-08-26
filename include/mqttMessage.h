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
#ifndef MQTT_MESSAGE_H
#define MQTT_MESSAGE_H

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include <memory>
#include <string>
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
class MQTTMessage
{
  public:
    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
    MQTTMessage(char* topic, char* payload, int payloadLen);
    ~MQTTMessage();
    std::string getTopic();
    std::string getPayload();
    int getPayloadLen();
    
  private:
    //----------------------------------------------------------------------------
    //  Class Atributes
    //----------------------------------------------------------------------------
    std::string mTopic = "";
    std::string mPayload = "";
    int mPayloadLen = 0;

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
};
#endif
