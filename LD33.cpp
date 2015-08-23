#include "stdafx.h"
#include "LD33.h"
#include "Monster.h"
#include "Augments.h"
#include "utility.h"
#include "Level.h"
#include "Angel.h"
#include "Particle_System.h"
#include "Hero.h"

TextActor* gold;

void Game::Initialize() {
  theWorld.Initialize(utility::Window_width,
                      utility::Window_height, "You Are The Monster", // gramr
                      1, 0, 1);
  theWorld.SetupPhysics(Vector2(0, -40));

  Level::Initialize();
  new Game::Mouse(); // for mouse events
  // enemy events
  Hero::e_listener = new Hero::Enemy_Listener(); // for enemy events
  theSwitchboard.SubscribeTo(Hero::e_listener,"FinishedHeroMovement");

  theOverseer = new Overseer();
  theWorld.Add(theOverseer);
  theWorld.SetBackgroundColor(Color(0.0117647f, 0.1098039f, 0.04313725f));
  
  Level::BG_Scroll::bg_scroll1 = new Actor();
  Level::BG_Scroll::bg_scroll2 = new Actor();
  Level::BG_Scroll::bg_scroll1->SetSprite("Images\\YATM-clouds.png");
  Level::BG_Scroll::bg_scroll1->SetSize(Vector2(MathUtil::PixelsToWorldUnits(829),
                                                MathUtil::PixelsToWorldUnits(320)));
  Level::BG_Scroll::bg_scroll2->SetSprite("Images\\YATM-clouds.png");
  Level::BG_Scroll::bg_scroll2->SetSize(Vector2(MathUtil::PixelsToWorldUnits(829),
                                                MathUtil::PixelsToWorldUnits(320)));
  theWorld.Add(Level::BG_Scroll::bg_scroll2);
  theWorld.Add(Level::BG_Scroll::bg_scroll1);
  
  FullScreenActor* bg = new FullScreenActor();
  bg->SetSprite("Images\\YATM-layer3.png");
  theWorld.Add(bg);

  
  bg = new FullScreenActor();
  bg->SetSprite("Images\\YATM-layer2.png");
  theWorld.Add(bg);
  
  
  bg = new FullScreenActor();
  bg->SetSprite("Images\\YATM-layer1.png");
  theWorld.Add(bg);
  

  thePlayer = new Player::Monster(Augments::Weapon_Type::Big_Sword);
  theWorld.Add(thePlayer);
  thePlayer->SetAlpha(1.0f);
  // collision system
  // left bottom corner wall
  auto t = new Level::Platform();
  theWorld.Add(t);
  t->SetPosition(MathUtil::ScreenToWorld(10,228));
  t->SetSize(MathUtil::PixelsToWorldUnits(100),
             MathUtil::PixelsToWorldUnits(71));
  t->SetColor(.30,.30,0);
  t->InitPhysics();
  t->SetAlpha(0);
  // bottom floor
  t = new Level::Platform();
  theWorld.Add(t);
  t->SetPosition(MathUtil::ScreenToWorld(230,264));
  t->SetSize(MathUtil::PixelsToWorldUnits(570),
             MathUtil::PixelsToWorldUnits(15));
  t->SetColor(.30,.30,0);
  t->InitPhysics();
  t->SetAlpha(0);
  // right bottom corner wall
  t = new Level::Platform();
  theWorld.Add(t);
  t->SetPosition(MathUtil::ScreenToWorld(450,228));
  t->SetSize(MathUtil::PixelsToWorldUnits(65),
             MathUtil::PixelsToWorldUnits(71));
  t->SetColor(.30,.30,0);
  t->InitPhysics();
  t->SetAlpha(0);

  // text actors
  gold = new TextActor();
  gold->SetPosition(50,50);
  gold->SetColor(.5,.5,0);

  Game::theKeep = new Augments::ShopKeep;
  theWorld.Add(theKeep);
}

Player::Monster* Game::thePlayer = nullptr;

Augments::ShopKeep* Game::theKeep = nullptr;
Game::Overseer* Game::theOverseer = nullptr;

void Game::Mouse::MouseDownEvent(Vec2i screenCoord, MouseButtonInput button) {
  auto world_coord = MathUtil::ScreenToWorld(screenCoord);
}

Game::Overseer::Overseer() { }

void Game::Overseer::Update(float dt) {
  if (Hero::theEnemy == nullptr && theKeep->time_left <= 0) {
    std::cout << "New Hero\n";
    Hero::theEnemy = new Hero::Enemy();
    theWorld.Add(Hero::theEnemy);
  }

  Particles::Update(dt);
  Level::BG_Scroll::Update(dt);
  Level::Leaves::Update(dt);
  
  gold->SetDisplayString("Gold: " + std::to_string(Game::thePlayer->R_Gold()));
}