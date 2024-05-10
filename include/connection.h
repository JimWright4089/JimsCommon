//----------------------------------------------------------------------------
//
//  Workfile: connection.h
//
//  Copyright: Jim Wright 2024
//
//  Notes:
//     This is the code for coneecting to things with a userid and password
//
//----------------------------------------------------------------------------
#ifndef CONNECTION_H
#define CONNECTION_H

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include <string>
#include <jsoncpp/json/json.h>

//----------------------------------------------------------------------------
//  Class Declarations
//----------------------------------------------------------------------------
//
// Class Name: Connection
// 
// Purpose:
//      The storage location for the SQL/MQTT Connection
//
//----------------------------------------------------------------------------
class Connection
{
  public:
    //----------------------------------------------------------------------------
    //  Const
    //----------------------------------------------------------------------------
    const std::string RANDOM_MARK = "<random>";

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
    Connection();
    Connection(const Json::Value connection);
    Connection(std::string jsonBody);  
    virtual ~Connection() {}
    void print(void);
    std::string getJson();
    bool loadFromFile(std::string fileName);

    std::string getHost();
    std::string getUser();
    std::string getPassword();
    std::string getDatabase();
    int getPort();
    std::string getClientID();
    std::string getMQTTAddress();
    std::string getSSLAddress();
    std::string getCACert();
    std::string getKeyStore();

  private:
    //----------------------------------------------------------------------------
    //  Class Atributes
    //----------------------------------------------------------------------------
    std::string mHost = "";
    std::string mUser = "";
    std::string mPassword = "";
    std::string mDatabase = "";
    int mPort = 0;
    std::string mClientID = "";
    std::string mCACert = "";
    std::string mKeyStore = "";

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
    void parseJson(const Json::Value connection);
};
#endif
