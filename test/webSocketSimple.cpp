//----------------------------------------------------------------------------
//
//  Workfile: webSocketSimple.cpp
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
#include <chrono>
#include <thread>
#include <vector>
#include <memory>
#include <future>
#include <iostream>
#include <vector>
#include <queue>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/use_future.hpp>
#include "randomSingleton.h"

using namespace std::chrono_literals;

//----------------------------------------------------------------------------
//  Global and Static data
//----------------------------------------------------------------------------
bool gRunTheThread = true;
std::vector<std::shared_ptr<std::thread>> gThreads;

const boost::asio::ip::address cAddress = boost::asio::ip::make_address("192.168.143.40");
const unsigned short cPort = static_cast<unsigned short>(4000);
boost::asio::io_context gIOContext{1};
boost::asio::ip::tcp::socket gSocket{gIOContext};

//----------------------------------------------------------------------------
//  Purpose:
//   Run the airline for one second
//
//  Notes:
//
//----------------------------------------------------------------------------
void stopThread(int s)
{
  std::cout << "Stopping app\n";
  gRunTheThread = false;
}

//----------------------------------------------------------------------------
//  Purpose:
//   Run the airline for one second
//
//  Notes:
//
//----------------------------------------------------------------------------
void doSession(boost::asio::ip::tcp::socket socket)
{
  int randSessionNumber = RandomSingleton::getInstance().next();
  std::string returnString = "";

  std::cout << "Start session for" << socket.remote_endpoint().address().to_string() << std::endl;
  try
  {
    // Construct the stream by moving in the socket
    boost::beast::websocket::stream<boost::asio::ip::tcp::socket> ws(std::move(socket));

    // Set a decorator to change the Server of the handshake
    ws.set_option(boost::beast::websocket::stream_base::decorator(
      [](boost::beast::websocket::response_type &res)
      {
          res.set(boost::beast::http::field::server,
                  std::string(BOOST_BEAST_VERSION_STRING) +
                  " websocket-server-sync");
      }));

    // Accept the websocket handshake
    ws.accept();

    while (true == gRunTheThread)
    {
      // This buffer will hold the incoming message
      boost::beast::flat_buffer buffer;
      returnString = "{\"sessionID\": \""+std::to_string(randSessionNumber)+"\"";
      returnString += ", \"messageID\": \"" + std::to_string(RandomSingleton::getInstance().next())+"\"}";

      std::cout << returnString << "\n";
      ws.write(boost::asio::buffer(returnString));

      std::this_thread::sleep_for(4000ms);
    }
  }
  catch (boost::beast::system_error const &se)
  {
      std::cout << "Error Boost: " << se.code().message() << std::endl;
  }
  catch (std::exception const &e)
  {
      std::cout << "Error: " << e.what() << std::endl;
  }
}

//----------------------------------------------------------------------------
//  Purpose:
//   Run the airline for one second
//
//  Notes:
//
//----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  int rc;

  signal(SIGTERM, stopThread);
  signal(SIGINT, stopThread);

  while (true == gRunTheThread)
  {
    try
    {
        // The acceptor receives incoming connections
        boost::asio::ip::tcp::acceptor acceptor{gIOContext, {cAddress, cPort}};

        acceptor.accept(gSocket);
        gThreads.push_back(std::shared_ptr<std::thread>(new std::thread(&doSession, std::move(gSocket))));
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
  }

  std::cout << "Clean up\n";

  for (std::shared_ptr<std::thread> curTread : gThreads)
  {
    curTread->join();
  }

  return rc;
}
