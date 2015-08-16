#include "stdafx.h"
#include "utility.h"
#include <random>

std::random_device utility::rand;
std::mt19937 utility::rgen(utility::rand());
std::uniform_real_distribution<> utility::dist(0, 100);

const int utility::Window_height = 1024,
          utility::Window_width  = 968;

int utility::Conv_To_World_X(int x) {
  return -13 - x / utility::Window_width;
}
int utility::Conv_To_World_Y(int x) {
  return -13 - x / utility::Window_height;
}


float utility::R_Rand() { return dist(rgen); }