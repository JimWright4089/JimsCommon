//----------------------------------------------------------------------------
//
//  Workfile: randomTest.cpp
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
#include "loggerSingleton.h"
#include "randomSingleton.h"

//----------------------------------------------------------------------------
//  Global and Static data
//----------------------------------------------------------------------------
RandomSingleton &gRandomNumber = RandomSingleton::getInstance();

//----------------------------------------------------------------------------
//  Purpose:
//   Entry point
//
//  Notes:
//
//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
  double count = 0;
  double addAmount = 0.01;
  int rc=0;

  LoggerSingleton::getInstance()->setFilename("randomTest",true);
  LoggerSingleton::getInstance()->setSeverity(SEVERITY_DEBUG);

  for(int i=0;i<10;i++)
  {
    LoggerSingleton::getInstance()->writeInfo("Rand number:" + std::to_string(gRandomNumber.next()));
  }
  for(int i=0;i<10;i++)
  {
    LoggerSingleton::getInstance()->writeInfo("Rand number:" + std::to_string(gRandomNumber.next(100)));
  }
  for(int i=0;i<10;i++)
  {
    LoggerSingleton::getInstance()->writeInfo("Rand number:" + std::to_string(gRandomNumber.next(200,400)));
  }

  return rc;
}
