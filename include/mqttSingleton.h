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
#ifndef MQTT_SINGLETON_H
#define MQTT_SINGLETON_H

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include "mqtt.h"

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
class MQTTSingleton
{
  public:
    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
    static MQTT &getInstance();
    
  private:
    //----------------------------------------------------------------------------
    //  Class Atributes
    //----------------------------------------------------------------------------
    static std::shared_ptr<MQTT> sInstance;

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
};
#endif