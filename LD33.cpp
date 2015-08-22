#include "stdafx.h"
#include "LD33.h"
#include "Monster.h"
#include "Augments.h"
#include "utility.h"
#include "Level.h"
#include "Angel.h"


void Game::Initialize() {
  theWorld.Initialize(utility::Window_height,
                      utility::Window_width, "You Are The Monster", // gramr
                      0, 0, 1);
  Level::Initialize();
  new Game::Mouse(); // for mouse events
  
  thePlayer = new Player::Monster(Augments::Head_Type::head,
                                  Augments::Weapon_Type::sword,
                                  Augments::Body_Type::body);
  
}

Player::Monster* Game::thePlayer = nullptr;



void Game::Mouse::MouseDownEvent(Vec2i screenCoord, MouseButtonInput button) {
  auto world_coord = MathUtil::ScreenToWorld(screenCoord);
  
}