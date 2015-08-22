#include "stdafx.h"
#include "utility.h"
#include <random>
#include <sstream>

std::random_device utility::rand;
std::mt19937 utility::rgen(utility::rand());
std::uniform_real_distribution<> utility::dist(0, 100);

const int utility::Window_height = 1024,
          utility::Window_width  = 968;



float utility::R_Rand() { return dist(rgen); }


void utility::Output(std::string text) {
  theWorld.GetConsole()->WriteToOutput(text);
}


  template <typename T>
  utility::R_Max(T x, T y) { return x > y ? x : y; }
  
  template <typename T>
  utility::R_Min(T x, T y) { return x < y ? x : y; }
  
  
  bool Find_Substr(std::string& out, std::string in) {
    if ( in == "" ) return false;
    int it = 0, hit_count = 0;
    for ( auto i : out ) {
      if ( in[it++] == i ) ++hit_count;
      else { it = 0; hit_count = 0; }
      if ( hit_count == in.size() ) break;
    }
    if ( hit_count == in.size() ) return true;
    
    return false;
  }
  
  template <typename T>
  T Str_To_Type(std::string x) {
    std::stringstream x;
    x >> o;
    T i;
    x << i;
    return i;
  }