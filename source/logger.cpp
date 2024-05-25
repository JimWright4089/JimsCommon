//----------------------------------------------------------------------------
//
//  Workfile: logger.cpp
//
//  Copyright: Jim Wright 2024
//
//  Notes:
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <cstdlib> 
#include <ctime> 
#include <boost/log/core.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include "logger.h"

using namespace std::chrono_literals;

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
Logger::Logger()
{

}

//----------------------------------------------------------------------------
//  Purpose:
//   Descructor
//
//  Notes:
//
//----------------------------------------------------------------------------
Logger::~Logger()
{
}

//----------------------------------------------------------------------------
//  Purpose:
//   Set the Filename
//
//  Notes:
//
//----------------------------------------------------------------------------
void Logger::setFilename(std::string filename, bool toLog)
{
  mFilename = filename;
  mLogToSTDOUT = toLog;

  boost::log::core::get()->flush();
  boost::log::core::get()->remove_all_sinks();

  boost::log::add_file_log
  (
    boost::log::keywords::file_name = mFilename+"_%N.log",
    boost::log::keywords::rotation_size = 10 * 1024 * 1024,
    boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
    boost::log::keywords::format = "[%TimeStamp%] [%Severity%]: %Message%",
    boost::log::keywords::open_mode = std::ios::out | std::ios::app
  );

  if(true == mLogToSTDOUT)
  {
    boost::log::add_console_log(std::cout, boost::log::keywords::format = "[%Severity%]: %Message%");
  }

  boost::log::core::get()->set_filter
  (
    boost::log::trivial::severity >= boost::log::trivial::info
  );

  boost::log::add_common_attributes();
  mOpen = true;

  int level = mLevel;

  setSeverity(SEVERITY_INFO);  
  writeInfo("------------------Log Start----------------");
  setSeverity(level);
}


//----------------------------------------------------------------------------
//  Purpose:
//   writeDebug
//
//  Notes:
//
//----------------------------------------------------------------------------
void Logger::writeDebug(std::string message)
{
  if(true == mOpen)
  {
    BOOST_LOG_SEV(mTheLog, boost::log::trivial::debug) << message;
  }
}

//----------------------------------------------------------------------------
//  Purpose:
//   writeInfo
//
//  Notes:
//
//----------------------------------------------------------------------------
void Logger::writeInfo(std::string message)
{
  if(true == mOpen)
  {
    BOOST_LOG_SEV(mTheLog, boost::log::trivial::info) << message;
  }
}

//----------------------------------------------------------------------------
//  Purpose:
//   writeWarning
//
//  Notes:
//
//----------------------------------------------------------------------------
void Logger::writeWarning(std::string message)
{
  if(true == mOpen)
  {
    BOOST_LOG_SEV(mTheLog, boost::log::trivial::warning) << message;
  }
}

//----------------------------------------------------------------------------
//  Purpose:
//   writeError
//
//  Notes:
//
//----------------------------------------------------------------------------
void Logger::writeError(std::string message)
{
  if(true == mOpen)
  {
    BOOST_LOG_SEV(mTheLog, boost::log::trivial::error) << message;
  }
}

//----------------------------------------------------------------------------
//  Purpose:
//   set the level
//
//  Notes:
//
//----------------------------------------------------------------------------
void Logger::setSeverity(int severity)
{
  if(true == mOpen)
  {
    switch(severity)
    {
      case SEVERITY_DEBUG:
        mLevel = boost::log::trivial::debug;
        break;
      case SEVERITY_INFO:
        mLevel = boost::log::trivial::info;
        break;
      case SEVERITY_WARNING:
        mLevel = boost::log::trivial::warning;
        break;
      default:
        mLevel = boost::log::trivial::error;
        break;
    }

    boost::log::core::get()->set_filter
    (
        boost::log::trivial::severity >= mLevel
    );
  }
}

//----------------------------------------------------------------------------
//  Purpose:
//   flush the log
//
//  Notes:
//
//----------------------------------------------------------------------------
void Logger::flush()
{
  boost::log::core::get()->flush();
}
