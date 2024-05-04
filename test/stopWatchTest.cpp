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
#include <memory>
#include <string>
#include <thread>
#include "stopWatch.h"

using namespace std::chrono_literals;

//----------------------------------------------------------------------------
//  Global and Static data
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Purpose:
//   Entry point
//
//  Notes:
//
//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
  int rc = 0;
  std::shared_ptr<StopWatch> testWatch = std::shared_ptr<StopWatch>(new StopWatch(static_cast<uint64_t>(2000)));

  testWatch->reset();

  while(false == testWatch->isExpired())
  {
    std::cout << "time left:" << testWatch->getTimeLeft() << "\n";
    std::this_thread::sleep_for(100ms);
  }

  return rc;
}
