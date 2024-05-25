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

//#define BOOST_LOG_DYN_LINK 1

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

//----------------------------------------------------------------------------
//  Purpose:
//   Init
//
//  Notes:
//
//----------------------------------------------------------------------------
void init()
{
    boost::log::add_file_log
    (
        boost::log::keywords::file_name = "/var/log/sample_%N.log",
        boost::log::keywords::rotation_size = 10 * 1024 * 1024,
        boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
        boost::log::keywords::format = "[%TimeStamp%]: %Message%",
        boost::log::keywords::open_mode = std::ios::out | std::ios::app
    );

    boost::log::core::get()->set_filter
    (
        boost::log::trivial::severity >= boost::log::trivial::info
    );
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
    init();
    boost::log::add_common_attributes();

    using namespace boost::log::trivial;
    boost::log::sources::severity_logger< severity_level > lg;

    BOOST_LOG_SEV(lg, trace) << "A trace severity message";
    BOOST_LOG_SEV(lg, debug) << "A debug severity message";
    BOOST_LOG_SEV(lg, info) << "An informational severity message";
    BOOST_LOG_SEV(lg, warning) << "A warning severity message";
    BOOST_LOG_SEV(lg, error) << "An error severity message";
    BOOST_LOG_SEV(lg, fatal) << "A fatal severity message";

    return 0;
}
