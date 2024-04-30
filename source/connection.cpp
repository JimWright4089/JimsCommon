//----------------------------------------------------------------------------
//
//  $Workfile: connection.cpp$
//
//  $Revision: X$
//
//  Modification History:
//  $Log:
//  $
//
//  Notes:
//     This is the code for coneecting to things with a userid and password
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include "connection.h"

//----------------------------------------------------------------------------
//  Purpose:
//   Constructor
//
//  Notes:
//
//----------------------------------------------------------------------------
Connection::Connection()
  : mHost(""),
    mUser(""),
    mPassword(""),
    mDatabase(""),
    mPort(0),
    mClientID(""),
    mCACert(""),
    mKeyStore("")
{
}

//----------------------------------------------------------------------------
//  Purpose:
//   Constructor
//
//  Notes:
//
//----------------------------------------------------------------------------
Connection::Connection(const Json::Value connection)
  : mHost(""),
    mUser(""),
    mPassword(""),
    mDatabase(""),
    mPort(0),
    mClientID(""),
    mCACert(""),
    mKeyStore("")
{
  parseJson(connection);
}

//----------------------------------------------------------------------------
//  Purpose:
//   Constructor
//
//  Notes:
//
//----------------------------------------------------------------------------
Connection::Connection(std::string jsonBody)
  : mHost(""),
    mUser(""),
    mPassword(""),
    mDatabase(""),
    mPort(0),
    mClientID(""),
    mCACert(""),
    mKeyStore("")
{
    Json::Value status;
    Json::Reader reader;
    reader.parse(jsonBody, status);  
    parseJson(status);
}

//----------------------------------------------------------------------------
//  Purpose:
//   Parse the json
//
//  Notes:
//
//----------------------------------------------------------------------------
void Connection::parseJson(const Json::Value connection)
{
  mHost = connection["host"].asString();
  mUser = connection["user"].asString();
  mPassword = connection["password"].asString();
  mDatabase = connection["database"].asString();
  mPort = connection["port"].asInt();
  mClientID = connection["clientid"].asString();
  mCACert = connection["cacert"].asString();
  mKeyStore = connection["keystore"].asString();
}

//----------------------------------------------------------------------------
//  Purpose:
//   Display values
//
//  Notes:
//
//----------------------------------------------------------------------------
void Connection::print(void)
{
  std::cout << "host     :" << mHost << "\n";
  std::cout << "user     :" << mUser << "\n";
  std::cout << "password :" << mPassword << "\n";
  std::cout << "database :" << mDatabase << "\n";
  std::cout << "port     :" << mPort << "\n";
  std::cout << "client id:" << mClientID << "\n";
  std::cout << "ca cert  :" << mCACert << "\n";
  std::cout << "keystr   :" << mKeyStore << "\n";
}

//----------------------------------------------------------------------------
//  Purpose:
//   return the json value
//
//  Notes:
//
//----------------------------------------------------------------------------
std::string Connection::getJson()
{
  Json::Value connection;

  connection["host"] = mHost;
  connection["user"] = mUser;
  connection["password"] = mPassword;
  connection["database"] = mDatabase;
  connection["port"] = mPort;
  connection["clientid"] = mClientID;
  connection["cacert"] = mCACert;
  connection["keystore"] = mKeyStore;

  return connection.toStyledString();
}

//----------------------------------------------------------------------------
//  Purpose:
//   return the json value
//
//  Notes:
//
//----------------------------------------------------------------------------
void Connection::loadFromFile(std::string fileName)
{
  Json::Value status;

  std::ifstream ifs(fileName);
  Json::Reader reader;

  reader.parse(ifs, status);
  parseJson(status);
}

//----------------------------------------------------------------------------
//  Purpose:
//   return the host
//
//  Notes:
//
//----------------------------------------------------------------------------
std::string Connection::getHost()
{
  return mHost;
}

//----------------------------------------------------------------------------
//  Purpose:
//   return the user
//
//  Notes:
//
//----------------------------------------------------------------------------
std::string Connection::getUser()
{
  return mUser;
}

//----------------------------------------------------------------------------
//  Purpose:
//   return the password
//
//  Notes:
//
//----------------------------------------------------------------------------
std::string Connection::getPassword()
{
  return mPassword;
}

//----------------------------------------------------------------------------
//  Purpose:
//   return the database
//
//  Notes:
//
//----------------------------------------------------------------------------
std::string Connection::getDatabase()
{
  return mDatabase;
}

//----------------------------------------------------------------------------
//  Purpose:
//   return the port
//
//  Notes:
//
//----------------------------------------------------------------------------
int Connection::getPort()
{
  return mPort;
}

//----------------------------------------------------------------------------
//  Purpose:
//   return the CA Certificate
//
//  Notes:
//
//----------------------------------------------------------------------------
std::string Connection::getCACert()
{
    return mCACert;
}

//----------------------------------------------------------------------------
//  Purpose:
//   return the Key Store
//
//  Notes:
//
//----------------------------------------------------------------------------
std::string Connection::getKeyStore()
{
    return mKeyStore;
}

//----------------------------------------------------------------------------
//  Purpose:
//   return the client id
//
//  Notes:
//
//----------------------------------------------------------------------------
std::string Connection::getClientID()
{
  return mClientID;
}

//----------------------------------------------------------------------------
//  Purpose:
//   return the address for mqtt
//
//  Notes:
//
//----------------------------------------------------------------------------
std::string Connection::getMQTTAddress()
{
  std::string returnString = std::string("tcp://")+mHost+std::string(":")+std::to_string(mPort);
  return returnString;
}

//----------------------------------------------------------------------------
//  Purpose:
//   return the ssl address for mqtt
//
//  Notes:
//
//----------------------------------------------------------------------------
std::string Connection::getSSLAddress()
{
  std::string returnString = std::string("ssl://")+mHost+std::string(":")+std::to_string(mPort);
  return returnString;
}


