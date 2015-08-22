#include "stdafx.h"
#include "LD33.h"
#include "Angel.h"


int main ( ) {
  Game::Initialize();
  theWorld.StartGame();
  theWorld.Destroy();
  return 0;
}