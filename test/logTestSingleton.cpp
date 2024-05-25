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
#include "loggerSingleton.h"

//----------------------------------------------------------------------------
//  Purpose:
//   Write Debug
//
//  Notes:
//
//----------------------------------------------------------------------------
void writeDebug()
{
  LoggerSingleton::getInstance()->writeDebug("debug");
}

//----------------------------------------------------------------------------
//  Purpose:
//   Write Info
//
//  Notes:
//
//----------------------------------------------------------------------------
void writeInfo()
{
  LoggerSingleton::getInstance()->writeInfo("info");
}
//----------------------------------------------------------------------------
//  Purpose:
//   Write Warning
//
//  Notes:
//
//----------------------------------------------------------------------------
void writeWarning()
{
  LoggerSingleton::getInstance()->writeWarning("warning");
}
//----------------------------------------------------------------------------
//  Purpose:
//   Write Error
//
//  Notes:
//
//----------------------------------------------------------------------------
void writeError()
{
  LoggerSingleton::getInstance()->writeError("error");
}

//----------------------------------------------------------------------------
//  Purpose:
//   Entry point
//
//  Notes:
//
//----------------------------------------------------------------------------
int main(int, char*[])
{
//  std::shared_ptr<Logger> log = std::shared_ptr<Logger>(new Logger());

  LoggerSingleton::getInstance()->setSeverity(SEVERITY_DEBUG);
  writeDebug();
  writeInfo();
  writeWarning();
  writeError();

  LoggerSingleton::getInstance()->setFilename("test",true);
  LoggerSingleton::getInstance()->setSeverity(SEVERITY_DEBUG);
  writeDebug();
  writeInfo();
  writeWarning();
  writeError();

  LoggerSingleton::getInstance()->setSeverity(SEVERITY_INFO);
  writeDebug();
  writeInfo();
  writeWarning();
  writeError();

  LoggerSingleton::getInstance()->setSeverity(SEVERITY_WARNING);
  writeDebug();
  writeInfo();
  writeWarning();
  writeError();

  LoggerSingleton::getInstance()->setSeverity(SEVERITY_ERROR);
  writeDebug();
  writeInfo();
  writeWarning();
  writeError();

  return 0;
}
