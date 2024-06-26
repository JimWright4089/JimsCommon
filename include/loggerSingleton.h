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
#ifndef LOGGER_SINGLETON_H
#define LOGGER_SINGLETON_H

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include "logger.h"

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
class LoggerSingleton
{
  public:
    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
    static std::shared_ptr<Logger> getInstance();
    
  private:
    //----------------------------------------------------------------------------
    //  Class Atributes
    //----------------------------------------------------------------------------
    static std::shared_ptr<Logger> sInstance;

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
};
#endif
