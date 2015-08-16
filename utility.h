#ifndef __UTILITY_H_
#define __UTILITY_H_

#include <random>

namespace utility {
  extern std::random_device rand;
  extern std::mt19937 rgen;
  extern std::uniform_real_distribution<> dist;

  extern const int Window_width;
  extern const int Window_height;

  int Conv_To_World_X(int);
  int Conv_To_World_Y(int);
  float R_Rand();
  
  void Console_Output(std::string);
};

#endif