#ifndef __UTILITY_H_
#define __UTILITY_H_

#include <random>

// general utility purposed things

namespace utility {
  /* rand eng stuff*/
  extern std::random_device rand;
  extern std::mt19937 rgen;
  extern std::uniform_real_distribution<> dist;

  extern const int Window_width;
  extern const int Window_height;
  // use MathUtil::ScreenToWorldto convert from local and world space 
  //int Conv_To_World_X(int);
  //int Conv_To_World_Y(int);
  
  // returns a random float from 0 to 100
  float R_Rand();
  
  // output to console (primarily for debugging purposes)
  void Output(std::string);
};

#endif