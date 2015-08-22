#include "stdafx.h"
#include "LD33.h"
#include "Monster.h"
#include "Augments.h"
#include "utility.h"
#include "Level.h"
#include "Angel.h"
#include "Hero.h"


void Game::Initialize() {
  theWorld.Initialize(utility::Window_width,
                      utility::Window_height, "You Are The Monster", // gramr
                      1, 0, 1);
  theWorld.SetupPhysics();
  Level::Initialize();
  new Game::Mouse(); // for mouse events
  
  thePlayer = new Player::Monster(Augments::Head_Type::head,
                                  Augments::Weapon_Type::sword,
                                  Augments::Body_Type::body);
  theWorld.Add(thePlayer);
  

  // enemy events
  Hero::e_listener = new Hero::Enemy_Listener(); // for enemy events
  theSwitchboard.SubscribeTo(Hero::e_listener,"FinishedHeroMovement");

  theOverseer = new Overseer();
  theWorld.Add(theOverseer);
}

Player::Monster* Game::thePlayer = nullptr;

Game::Overseer* Game::theOverseer = nullptr;


void Game::Mouse::MouseDownEvent(Vec2i screenCoord, MouseButtonInput button) {
  auto world_coord = MathUtil::ScreenToWorld(screenCoord);
}


void Game::Overseer::Update(float dt) {
  if ( Hero::theEnemyIntro == nullptr &&
       Hero::theEnemy      == nullptr ) {
    Hero::theEnemyIntro = new Hero::Enemy_Intro();
    theWorld.Add(Hero::theEnemyIntro);
  }
}