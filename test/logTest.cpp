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
#include "logger.h"

//----------------------------------------------------------------------------
//  Purpose:
//   Entry point
//
//  Notes:
//
//----------------------------------------------------------------------------
int main(int, char*[])
{
  std::shared_ptr<Logger> log = std::shared_ptr<Logger>(new Logger());

  log->setFilename("test");

  log->setSeverity(SEVERITY_DEBUG);
  log->writeDebug("Debug");
  log->writeInfo("Info");
  log->writeWarning("Warning");
  log->writeError("Error");

  log->setSeverity(SEVERITY_INFO);
  log->writeDebug("Debug");
  log->writeInfo("Info");
  log->writeWarning("Warning");
  log->writeError("Error");

  log->setSeverity(SEVERITY_WARNING);
  log->writeDebug("Debug");
  log->writeInfo("Info");
  log->writeWarning("Warning");
  log->writeError("Error");

  log->setSeverity(SEVERITY_ERROR);
  log->writeDebug("Debug");
  log->writeInfo("Info");
  log->writeWarning("Warning");
  log->writeError("Error");

  return 0;
}
