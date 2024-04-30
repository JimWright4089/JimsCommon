//----------------------------------------------------------------------------
//
//  $Workfile: mySqlTest.cpp$
//
//  $Revision: X$
//
//  $Copyright: Jim Wright 2024
//
//  Modification History:
//  $Log:
//  $
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
RandomSingleton &gRandomNumber = RandomSingleton::GetInstance();

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

  for(int i=0;i<30;i++)
  {
    std::cout << "Rand number:" << gRandomNumber.Next(100) << "\n";
  }

  return rc;
}
