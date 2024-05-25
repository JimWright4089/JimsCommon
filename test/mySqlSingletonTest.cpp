//----------------------------------------------------------------------------
//
//  Workfile: mySqlTest.cpp
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
#include <iostream>
#include <string>
#include "math.h"
#include "loggerSingleton.h"
#include "connection.h"
#include "mySqlDBSingleton.h"

//----------------------------------------------------------------------------
//  Global and Static data
//----------------------------------------------------------------------------
bool gVerboseDB = true;

//----------------------------------------------------------------------------
//  Purpose:
//   Insert a row
//
//  Notes:
//
//----------------------------------------------------------------------------
void insert(double value)
{
  MySQLDB database = MySQLDBSingleton::getInstance();
  int returnValue = 0;
  std::string sqlCommand = "insert into sin_test(sin_value) values(" + std::to_string(value) + ");";
  database.executeStatement(sqlCommand, returnValue);
  LoggerSingleton::getInstance()->writeInfo("Return value:" + std::to_string(returnValue));
}

//----------------------------------------------------------------------------
//  Purpose:
//   delete all the rows
//
//  Notes:
//
//----------------------------------------------------------------------------
void deleteAll(void)
{
  MySQLDB database = MySQLDBSingleton::getInstance();
  int returnValue = 0;
  std::string sqlCommand = "delete from sin_test;";
  database.executeStatement(sqlCommand, returnValue);
  LoggerSingleton::getInstance()->writeInfo("Return value:" + std::to_string(returnValue));
}

//----------------------------------------------------------------------------
//  Purpose:
//   Entry point
//
//  Notes:
//
//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
  double count = 0;
  double addAmount = 0.01;
  int rc=0;

  LoggerSingleton::getInstance()->setFilename("mySqlSingletonTest",true);
  LoggerSingleton::getInstance()->setSeverity(SEVERITY_DEBUG);

  if(argc != 2)
  {
    LoggerSingleton::getInstance()->writeError("Usage: mySqlSingletonTest <mysql connection file> ");
    return -1;
  }

  MySQLDB database = MySQLDBSingleton::getInstance();
  std::string sqlFilename(argv[1]);

  database.loadConfiguration(sqlFilename);
  database.openDatabase();

  deleteAll();

  for(int i=0;i<30;i++)
  {
    count+=addAmount;

    insert(sin(count));
    usleep(100000L);
  }

  return rc;
}
