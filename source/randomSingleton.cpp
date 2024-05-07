//----------------------------------------------------------------------------
//
//  Workfile: randomSingleton.cpp
//
//  Copyright: Jim Wright 2024
//
//  Notes:
//     This is the code for returning random numbers
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include <cstdlib> 
#include <ctime> 
#include <iostream>
#include "randomSingleton.h"

//----------------------------------------------------------------------------
//  Global and Static data
//----------------------------------------------------------------------------
std::shared_ptr<RandomSingleton> RandomSingleton::sInstance = nullptr;

//----------------------------------------------------------------------------
//  Purpose:
//   Constructor
//
//  Notes:
//
//----------------------------------------------------------------------------
RandomSingleton::RandomSingleton()
{
    srand((unsigned)time(0));
}

//----------------------------------------------------------------------------
//  Purpose:
//   Return the pointer to the real object
//
//  Notes:
//
//----------------------------------------------------------------------------
RandomSingleton &RandomSingleton::getInstance()
{
    if (sInstance == nullptr) 
    {
        sInstance = std::shared_ptr<RandomSingleton>(new RandomSingleton());
    }
    return *sInstance;
}

//----------------------------------------------------------------------------
//  Purpose:
//   Return a number
//
//  Notes:
//
//----------------------------------------------------------------------------
int RandomSingleton::next()
{
    return rand();
}

//----------------------------------------------------------------------------
//  Purpose:
//   Return a number from 0 to total-1
//
//  Notes:
//
//----------------------------------------------------------------------------
int RandomSingleton::next(int total)
{
  if (0 == total)
  {
    return 0;
  }
  return rand()%total;
}

//----------------------------------------------------------------------------
//  Purpose:
//   Return a number from start to end-1
//
//  Notes:
//
//----------------------------------------------------------------------------
int RandomSingleton::next(int start, int end)
{
  int total = end - start;

  if (0 == total)
  {
    return start;
  }

  return start+(rand()%total);
}

