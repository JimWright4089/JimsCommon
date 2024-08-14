//----------------------------------------------------------------------------
//
//  $Workfile: mySqlDatabase.cpp$
//
//  Copyright: Jim Wright 2024
//
//  Notes:
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <cppconn/statement.h>
#include "loggerSingleton.h"
#include "mySqlDB.h"

//----------------------------------------------------------------------------
//  Global and Static data
//----------------------------------------------------------------------------
std::mutex MySQLDB::mSQLMutex;

//----------------------------------------------------------------------------
//  Purpose:
//   Constructor
//
//  Notes:
//
//----------------------------------------------------------------------------
MySQLDB::MySQLDB() :
  mDriver(NULL),
  mConnect(NULL)
{

}

//----------------------------------------------------------------------------
//  Purpose:
//   destructor
//
//  Notes:
//
//----------------------------------------------------------------------------
MySQLDB::~MySQLDB()
{
  if(NULL != mConnect)
  {
    delete mConnect;
  }
}

//----------------------------------------------------------------------------
//  Purpose:
//   load configuration from file
//
//  Notes:
//
//----------------------------------------------------------------------------
void MySQLDB::loadConfiguration(std::string fileName)
{
  std::string sqlFilename(fileName);
  if(false == mConnection.loadFromFile(sqlFilename))
  {
    LoggerSingleton::getInstance()->writeError("The connection is blank");
  }

  mConnection.print();  
}

//----------------------------------------------------------------------------
//  Purpose:
//   Open the database
//
//  Notes:
//
//----------------------------------------------------------------------------
void MySQLDB::openDatabase()
{
  if(NULL == mDriver)
  {
    mDriver = sql::mysql::get_driver_instance();
  }

  if((""==mConnection.getHost())&&(""==mConnection.getUser()))
  {
    LoggerSingleton::getInstance()->writeError("The connection is blank not connecting to database");
  }
  else
  {
    if(nullptr == mConnect)
    {
      mConnect = mDriver->connect(
            mConnection.getHost().c_str(),
            mConnection.getUser().c_str(),
            mConnection.getPassword().c_str());

      mConnect->setSchema(mConnection.getDatabase().c_str());
    }
  }

}

//----------------------------------------------------------------------------
//  Purpose:
//   Run a query
//
//  Notes:
//
//----------------------------------------------------------------------------
void MySQLDB::executeStatement(std::string sqlCommand, int &returnValue)
{
  returnValue = EXIT_FAILURE;

  if(NULL == mConnect)
  {
    return;
  }

  LoggerSingleton::getInstance()->writeDebug("executeStatement:{" + sqlCommand + "}");

  try 
  {
    std::lock_guard<std::mutex> guard(mSQLMutex);
    std::shared_ptr<sql::Statement> statement = std::shared_ptr<sql::Statement>(mConnect->createStatement());
    statement->execute(sqlCommand.c_str());
    returnValue = EXIT_SUCCESS;
  } 
  catch (sql::SQLException &e) 
  {
    LoggerSingleton::getInstance()->writeError("# ERR: " + std::string(e.what())+ 
      " (MySQL error code: " + std::to_string(e.getErrorCode()) + ", SQLState: " + e.getSQLState() + " )");

    returnValue = e.getErrorCode();
  }
}

//----------------------------------------------------------------------------
//  Purpose:
//   Run a query
//
//  Notes:
//
//----------------------------------------------------------------------------
std::shared_ptr<sql::ResultSet> MySQLDB::executeStatementWithResult(std::string sqlCommand, int &returnValue)
{
  returnValue = EXIT_FAILURE;

  if(NULL == mConnect)
  {
    return nullptr;
  }

  LoggerSingleton::getInstance()->writeDebug("executeStatementWithResult:" + sqlCommand + "}");

  try 
  {
    std::lock_guard<std::mutex> guard(mSQLMutex);
    returnValue = EXIT_SUCCESS;
    std::shared_ptr<sql::Statement> statement = std::shared_ptr<sql::Statement>(mConnect->createStatement());
    return std::shared_ptr<sql::ResultSet>(statement->executeQuery(sqlCommand.c_str()));
  } 
  catch (sql::SQLException &e) 
  {
    LoggerSingleton::getInstance()->writeError("# ERR: " + std::string(e.what())+ 
      " (MySQL error code: " + std::to_string(e.getErrorCode()) + ", SQLState: " + e.getSQLState() + " )");

    returnValue = e.getErrorCode();
  }

  return NULL;
}

//----------------------------------------------------------------------------
//  Purpose:
//   Run a query that returns a number
//
//  Notes:
//
//----------------------------------------------------------------------------
int MySQLDB::getInt(std::string sqlCommand, int &returnValue)
{
  returnValue = EXIT_FAILURE;
  std::shared_ptr<sql::ResultSet> results = std::shared_ptr<sql::ResultSet>(executeStatementWithResult(sqlCommand, returnValue));

  if(EXIT_SUCCESS != returnValue)
  {
    return -1;
  }

  int theCount;

  while (results->next()) 
  {
    return results->getInt(1);
  }    
  return -1;
}

//----------------------------------------------------------------------------
//  Purpose:
//   Run a query that returns a number
//
//  Notes:
//
//----------------------------------------------------------------------------
std::string MySQLDB::getString(std::string sqlCommand, int &returnValue)
{
  std::shared_ptr<sql::ResultSet> results = std::shared_ptr<sql::ResultSet> (executeStatementWithResult(sqlCommand, returnValue));

  if(EXIT_SUCCESS != returnValue)
  {
    return std::string("");
  }

  while (results->next()) 
  {
    return std::string(results->getString(1));
  }    
  return std::string("");
}
