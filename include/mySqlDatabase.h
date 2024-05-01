//----------------------------------------------------------------------------
//
//  Workfile: mySQLDatabae.h
//
//  Copyright: Jim Wright 2024
//
//  Notes:
//     This is the mySQL database handler
//
//----------------------------------------------------------------------------
#ifndef MY_SQL_DATABASE_H
#define MY_SQL_DATABASE_H

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include <string>
#include "connection.h"
#include "mysql_driver.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>

//----------------------------------------------------------------------------
//  Class Declarations
//----------------------------------------------------------------------------
//
// Class Name: MySQLDatabase
// 
// Purpose:
//      Handle MySQL Databases
//
//----------------------------------------------------------------------------
class MySQLDatabase
{
  public:

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
    static MySQLDatabase &getInstance();
    void loadConfiguration(std::string fileName);
    void openDatabase();
    std::shared_ptr<sql::ResultSet> executeStatementWithResult(std::string sqlCommand, int &returnValue);
    void executeStatement(std::string sqlCommand, int &returnValue);
    int getInt(std::string sqlCommand, int &returnValue);
    std::string getString(std::string sqlCommand, int &returnValue);
    ~MySQLDatabase();

  private:
    //----------------------------------------------------------------------------
    //  Class Atributes
    //----------------------------------------------------------------------------
    static std::shared_ptr<MySQLDatabase> sInstance;
    Connection mConnection;
    sql::Driver* mDriver;
    sql::Connection* mConnect;

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
    MySQLDatabase();  
};
#endif
