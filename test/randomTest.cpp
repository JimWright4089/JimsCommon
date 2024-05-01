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

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include <iostream>
#include <string>
#include "randomSingleton.h"

//----------------------------------------------------------------------------
//  Global and Static data
//----------------------------------------------------------------------------
RandomSingleton &gRandomNumber = RandomSingleton::getInstance();

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

  for(int i=0;i<10;i++)
  {
    std::cout << "Rand number:" << gRandomNumber.next() << "\n";
  }
  for(int i=0;i<10;i++)
  {
    std::cout << "Rand number:" << gRandomNumber.next(100) << "\n";
  }
  for(int i=0;i<10;i++)
  {
    std::cout << "Rand number:" << gRandomNumber.next(200,400) << "\n";
  }

  return rc;
}
