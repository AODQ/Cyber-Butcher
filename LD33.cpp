#include "LD33.h"
#include "Monster.h"
#include "Augments.h"
#include "utility.h"


void Game::Initialize() {
  theWorld.Initialize(utility::Window_height,
                      utility::Window_width, "You Are The Monster", // gramr
                      0, 0, 1)
  Level::Initialize();
  new Mouse(); // for mouse events
  
  thePlayer = new Monster(Augments::Head_Type::head,
                          Augments::Weapon_Type::weapon,
                          Augments::Body_Type::body);
  
}

Player::Monster* Game::thePlayer = nullptr;



void Mouse::MouseDownEvent(Vec2i screenCoord, MouseButtonInput button) {
  auto world_coord = MathUtil::ScreenToWorld(screenCoord);
  
}