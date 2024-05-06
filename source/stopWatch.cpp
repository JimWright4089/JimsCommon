//----------------------------------------------------------------------------
//
//  Workfile: mqtt.cpp
//
//  Copyright: Jim Wright 2024
//
//  Notes:
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include "stopWatch.h"

//----------------------------------------------------------------------------
//  Global and Static data
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Purpose:
//   Constructor
//
//  Notes:
//
//----------------------------------------------------------------------------
StopWatch::StopWatch() :
  mLastTime(0),
  mWaitTime(1000),
  mIsSeconds(false)
{
}

//----------------------------------------------------------------------------
//  Purpose:
//   Constructor
//
//  Notes:
//
//----------------------------------------------------------------------------
StopWatch::StopWatch(uint64_t waitTime) :
  mLastTime(0),
  mWaitTime(waitTime),
  mIsSeconds(false)
{
}

//----------------------------------------------------------------------------
//  Purpose:
//   Constructor
//
//  Notes:
//
//----------------------------------------------------------------------------
StopWatch::StopWatch(bool isSeconds) :
  mLastTime(0),
  mWaitTime(1000),
  mIsSeconds(isSeconds)
{
}

//----------------------------------------------------------------------------
//  Purpose:
//   Constructor
//
//  Notes:
//
//----------------------------------------------------------------------------
StopWatch::StopWatch(uint64_t waitTime, bool isSeconds) :
  mLastTime(0),
  mWaitTime(waitTime),
  mIsSeconds(isSeconds)
{
}

//----------------------------------------------------------------------------
//  Purpose:
//   Return the milliseconds
//
//  Notes:
//
//----------------------------------------------------------------------------
uint64_t StopWatch::now(void)
{
  std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
    std::chrono::system_clock::now().time_since_epoch());

  return ms.count();
}

//----------------------------------------------------------------------------
//  Purpose:
//   Return the milliseconds
//
//  Notes:
//
//----------------------------------------------------------------------------
uint64_t StopWatch::nowInSeconds(void)
{
  std::chrono::seconds secs = std::chrono::duration_cast< std::chrono::seconds >(
    std::chrono::system_clock::now().time_since_epoch());

  return secs.count();
}

//----------------------------------------------------------------------------
//  Purpose:
//   Getter Setter
//
//  Notes:
//
//----------------------------------------------------------------------------
void StopWatch::setTime(uint64_t waitTime)
{
  mWaitTime = waitTime;
}

//----------------------------------------------------------------------------
//  Purpose:
//   Is the timer finished
//
//  Notes:
//
//----------------------------------------------------------------------------
bool StopWatch::isExpired(void)
{
  if (false == mIsSeconds)
  {
    if ((now() - mLastTime) > mWaitTime)
    {
      return true;
    }
  }
  else
  {
    if ((nowInSeconds() - mLastTime) > mWaitTime)
    {
      return true;
    }
  }
  return false;
}

//----------------------------------------------------------------------------
//  Purpose:
//   Reset the time back to start
//
//  Notes:
//
//----------------------------------------------------------------------------
void StopWatch::reset(void)
{
  if (false == mIsSeconds)
  {
    mLastTime = now();
  }
  else
  {
    mLastTime = nowInSeconds();
  }
}

//----------------------------------------------------------------------------
//  Purpose:
//   Return the amount of time left
//
//  Notes:
//
//----------------------------------------------------------------------------
int64_t StopWatch::getTimeLeft()
{
  if (false == mIsSeconds)
  {
    return mWaitTime - (now() - mLastTime);
  }
  else
  {
    return mWaitTime - (nowInSeconds() - mLastTime);
  }
}