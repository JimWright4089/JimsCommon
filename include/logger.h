//----------------------------------------------------------------------------
//
//  Workfile: logger.h$
//
//  Copyright: Jim Wright 2024
//
//  Notes:
//     This is a handles the log
//
//----------------------------------------------------------------------------
#ifndef LOGGER_H
#define LOGGER_H

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include <string>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

//namespace logging = boost::log;
//namespace src = boost::log::sources;
//namespace sinks = boost::log::sinks;
//namespace keywords = boost::log::keywords;

const int SEVERITY_DEBUG = 3;
const int SEVERITY_INFO = 2;
const int SEVERITY_WARNING = 1;
const int SEVERITY_ERROR = 0;

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
class Logger
{
  public:
    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
    Logger();
    ~Logger();

    void setFilename(std::string filename, bool toLog=false);
    void writeDebug(std::string message);
    void writeInfo(std::string message);
    void writeWarning(std::string message);
    void writeError(std::string message);
    void setSeverity(int severity);

  private:
    //----------------------------------------------------------------------------
    //  Class Atributes
    //----------------------------------------------------------------------------
    std::string mFilename = "unknown";
    boost::log::sources::severity_logger< boost::log::v2s_mt_posix::trivial::severity_level > mTheLog;
    boost::log::v2s_mt_posix::trivial::severity_level mLevel = boost::log::trivial::error;
    bool mOpen=false;
    bool mLogToSTDOUT = false;

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
};
#endif
