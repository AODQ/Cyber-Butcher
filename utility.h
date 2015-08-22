#include <random>
#include "stdafx.h"

#ifndef __UTILITY_H_
#define __UTILITY_H_

// general utility purposed things

namespace utility {
  /* rand eng stuff*/
  extern std::random_device rand;
  extern std::mt19937 rgen;
  extern std::uniform_real_distribution<> dist;

  extern const int Window_width;
  extern const int Window_height;
  
  // returns a random float from 0 to 100
  float R_Rand();
  
  // output to console (primarily for debugging purposes)
  void Output(std::string);
  
  template <typename T>
  T R_Max(T x, T y);
  
  template <typename T>
  T R_Min(T x, T y);
  
  
  bool Find_Substr(std::string&, std::string);
  template <typename T>
  T Str_To_Type(std::string x);
};

#endif