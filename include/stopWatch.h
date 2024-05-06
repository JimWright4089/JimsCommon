//----------------------------------------------------------------------------
//
//  Workfile: stopWatch.h$
//
//  Copyright: Jim Wright 2024
//
//  Notes:
//     This is a singleton publishing and subscribing to mqtt topics
//
//----------------------------------------------------------------------------
#ifndef STOP_WATCH_H
#define STOP_WATCH_H

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include <chrono>

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
class StopWatch
{
  public:
    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
    StopWatch();
    StopWatch(uint64_t waitTime);
    StopWatch(bool isSeconds);
    StopWatch(uint64_t waitTime, bool isSeconds);
    void setTime(uint64_t waitTime);
    bool isExpired(void);
    void reset(void);
    int64_t getTimeLeft(void);
    static uint64_t now(void);
    static uint64_t nowInSeconds(void);
    
  private:
    //----------------------------------------------------------------------------
    //  Class Atributes
    //----------------------------------------------------------------------------
    uint64_t mLastTime;
    uint64_t mWaitTime;
    bool mIsSeconds;

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
};
#endif
