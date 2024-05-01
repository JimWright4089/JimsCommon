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
#include "mySqlDatabase.h"

//----------------------------------------------------------------------------
//  Global and Static data
//----------------------------------------------------------------------------
extern bool gVerboseDB;
std::shared_ptr<MySQLDatabase> MySQLDatabase::sInstance = nullptr;

//----------------------------------------------------------------------------
//  Purpose:
//   GetInstance
//
//  Notes:
//
//----------------------------------------------------------------------------
MySQLDatabase &MySQLDatabase::getInstance()
{
    if (sInstance == nullptr) 
    {
        sInstance = std::shared_ptr<MySQLDatabase>(new MySQLDatabase());
    }
    return *sInstance;
}

//----------------------------------------------------------------------------
//  Purpose:
//   Constructor
//
//  Notes:
//
//----------------------------------------------------------------------------
MySQLDatabase::MySQLDatabase() :
  mDriver(NULL),
  mConnect(NULL)
{

}

//----------------------------------------------------------------------------
//  Purpose:
//   load configuration from file
//
//  Notes:
//
//----------------------------------------------------------------------------
void MySQLDatabase::loadConfiguration(std::string fileName)
{
  std::string sqlFilename(fileName);
  mConnection.loadFromFile(sqlFilename);
}

//----------------------------------------------------------------------------
//  Purpose:
//   Open the database
//
//  Notes:
//
//----------------------------------------------------------------------------
void MySQLDatabase::openDatabase()
{
  if(NULL == mDriver)
  {
    mDriver = sql::mysql::get_driver_instance();
  }

  if(nullptr == mConnect)
  {
    mConnect = mDriver->connect(
          mConnection.getHost().c_str(),
          mConnection.getUser().c_str(),
          mConnection.getPassword().c_str());

    mConnect->setSchema(mConnection.getDatabase().c_str());
  }

}

//----------------------------------------------------------------------------
//  Purpose:
//   destructor
//
//  Notes:
//
//----------------------------------------------------------------------------
MySQLDatabase::~MySQLDatabase()
{
  if(NULL != mConnect)
  {
    delete mConnect;
  }
}

//----------------------------------------------------------------------------
//  Purpose:
//   Run a query
//
//  Notes:
//
//----------------------------------------------------------------------------
void MySQLDatabase::executeStatement(std::string sqlCommand, int &returnValue)
{
  returnValue = EXIT_FAILURE;

  if(true == gVerboseDB)
  {
    std::cout << "executeStatement:{" + sqlCommand + "}" << std::endl;
  }

  try 
  {
    std::shared_ptr<sql::Statement> statement = std::shared_ptr<sql::Statement>(mConnect->createStatement());
    statement->execute(sqlCommand.c_str());
    returnValue = EXIT_SUCCESS;
  } 
  catch (sql::SQLException &e) 
  {
    std::cout << "# ERR: SQLException in " << __FILE__;
    /* Use what() (derived from std::runtime_error) to fetch the error message */
    std::cout << "# ERR: " << e.what();
    std::cout << " (MySQL error code: " << e.getErrorCode();
    std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;

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
std::shared_ptr<sql::ResultSet> MySQLDatabase::executeStatementWithResult(std::string sqlCommand, int &returnValue)
{
  returnValue = EXIT_FAILURE;

  if(true == gVerboseDB)
  {
    std::cout << "executeStatementWithResult:{" + sqlCommand + "}" << std::endl;
  }

  try 
  {
    returnValue = EXIT_SUCCESS;
    std::shared_ptr<sql::Statement> statement = std::shared_ptr<sql::Statement>(mConnect->createStatement());
    return std::shared_ptr<sql::ResultSet>(statement->executeQuery(sqlCommand.c_str()));
  } 
  catch (sql::SQLException &e) 
  {
    std::cout << "# ERR: SQLException in " << __FILE__;
    /* Use what() (derived from std::runtime_error) to fetch the error message */
    std::cout << "# ERR: " << e.what();
    std::cout << " (MySQL error code: " << e.getErrorCode();
    std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;

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
int MySQLDatabase::getInt(std::string sqlCommand, int &returnValue)
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
std::string MySQLDatabase::getString(std::string sqlCommand, int &returnValue)
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
