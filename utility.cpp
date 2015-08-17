#include "stdafx.h"
#include "utility.h"
#include <random>

std::random_device utility::rand;
std::mt19937 utility::rgen(utility::rand());
std::uniform_real_distribution<> utility::dist(0, 100);

const int utility::Window_height = 1024,
          utility::Window_width  = 968;



float utility::R_Rand() { return dist(rgen); }


void utility::Output(std::string text) {
  theWorld.GetConsole()->WriteToOutput(text);
}