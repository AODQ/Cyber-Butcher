#include "stdafx.h"
#include "Angel.h"
#include "Augments.h"
#include "Monster.h"
#include "Level.h"
#include "LD33.h"


int main ( ) {
  Game::Initialize();
  theWorld.StartGame();
  theWorld.Destroy();
  return 0;
}