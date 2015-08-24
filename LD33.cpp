#include "stdafx.h"
#include "LD33.h"
#include "Monster.h"
#include "Augments.h"
#include "utility.h"
#include "Level.h"
#include "Angel.h"
#include "Particle_System.h"
#include "Sounds.h"
#include "Hero.h"

TextActor* gold;

bool Game::in_menu = 0;

void Game::Initialize() {
  in_menu = 1;
  theWorld.Initialize(utility::Window_width,
                      utility::Window_height, "You Are The Monster", // gramr
                      1, 0, 1);
  theWorld.SetupPhysics(Vector2(0, -40));
  theWorld.SetSideBlockers(1);
  Level::Initialize();
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
  new Game::Mouse(); // for mouse events
  // enemy events
  Hero::e_listener = new Hero::Enemy_Listener(); // for enemy events
  theSwitchboard.SubscribeTo(Hero::e_listener,"FinishedHeroMovement");

  theOverseer = new Overseer();
  theWorld.Add(theOverseer);
  theWorld.SetBackgroundColor(Color(0.0117647f, 0.1098039f, 0.04313725f));
  Initialize_Game();
}

void Game::Initialize_Game() {
  in_menu = 0;
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
  // sounds
  Sounds::Load_Sounds();

}

Player::Monster* Game::thePlayer = nullptr;

Augments::ShopKeep* Game::theKeep = nullptr;
Game::Overseer* Game::theOverseer = nullptr;

void Game::Mouse::MouseDownEvent(Vec2i screenCoord, MouseButtonInput button) {
  auto world_coord = MathUtil::ScreenToWorld(screenCoord);
}

Game::Overseer::Overseer() {
  level = 0;
  menu_start = new Actor();
  menu_controls = new Actor();
  menu_exit = new Actor();
  menu_select = new Actor();
  menu_start->SetSize(MathUtil::PixelsToWorldUnits(150),
                      MathUtil::PixelsToWorldUnits(50));
  menu_controls->SetSize(MathUtil::PixelsToWorldUnits(150),
                      MathUtil::PixelsToWorldUnits(50));
  menu_exit->SetSize(MathUtil::PixelsToWorldUnits(150),
                      MathUtil::PixelsToWorldUnits(50));
  menu_select->SetSize(MathUtil::PixelsToWorldUnits(150),
                        MathUtil::PixelsToWorldUnits(50));
  menu_start->SetPosition(0,0);
  menu_controls->SetPosition(0,MathUtil::PixelsToWorldUnits(60));
  menu_exit->SetPosition(0,MathUtil::PixelsToWorldUnits(120));
  menu_select->SetPosition(0,0);

}

void Game::Overseer::Start_Game() {
  menu_select->Destroy();
  menu_exit->Destroy();
  menu_controls->Destroy();
  menu_start->Destroy();
  in_menu = 0;
}

void Game::Overseer::Update(float dt) {
  if ( in_menu ) {
    switch ( selected_icon ) {
      case 0: // menu start
        menu_select->SetPosition(0,0);
      break;
    }
  } else { 
    if (Hero::theEnemy == nullptr && theKeep->time_left <= 0 ) {
      std::cout << "New Hero\n";
      Hero::theEnemy = new Hero::Enemy();
      Hero::theEnemy->Add_Health(level*20);
      theWorld.Add(Hero::theEnemy);
    }

    Particles::Update(dt);
    Level::BG_Scroll::Update(dt);
    Level::Leaves::Update(dt);
  
    gold->SetDisplayString("Gold: " + std::to_string(Game::thePlayer->R_Gold()));

    Sounds::Update(dt);
  }
}