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

  // enemy events
  Hero::e_listener = new Hero::Enemy_Listener(); // for enemy events
  theSwitchboard.SubscribeTo(Hero::e_listener,"FinishedHeroMovement");

  theOverseer = new Overseer();
  theWorld.Add(theOverseer);
  theWorld.SetBackgroundColor(Color(0.0117647f, 0.1098039f, 0.04313725f));

  Game::theMouse = new Mouse();

  // collision
  // left bottom corner wall
  auto t = new Level::Platform();
  theWorld.Add(t);
  t->SetPosition(MathUtil::ScreenToWorld(10,228));
  std::cout << "t1: " << t->GetPosition().X << " " << t->GetPosition().Y << '\n';
  t->SetSize(MathUtil::PixelsToWorldUnits(100),
             MathUtil::PixelsToWorldUnits(71));
  t->SetColor(.30,.30,0);
  t->InitPhysics();
  t->SetAlpha(0);
  // bottom floor
  t = new Level::Platform();
  theWorld.Add(t);
  t->SetPosition(MathUtil::ScreenToWorld(230,264));
  std::cout << "t2: " << t->GetPosition().X << " " << t->GetPosition().Y << '\n';
  t->SetSize(MathUtil::PixelsToWorldUnits(570),
             MathUtil::PixelsToWorldUnits(15));
  t->SetColor(.30,.30,0);
  t->InitPhysics();
  t->SetAlpha(0);
  // right bottom corner wall
  t = new Level::Platform();
  theWorld.Add(t);
  t->SetPosition(MathUtil::ScreenToWorld(450,228));
  std::cout << "t2: " << t->GetPosition().X << " " << t->GetPosition().Y << '\n';
  t->SetSize(MathUtil::PixelsToWorldUnits(65),
             MathUtil::PixelsToWorldUnits(71));
  t->SetColor(.30,.30,0);
  t->InitPhysics();
  t->SetAlpha(0);

  glfwGetWindowSize(theWorld.GetMainWindow(), &utility::True_width, &utility::True_height);
}

void Game::Initialize_Game() {
  in_menu = 0;

  ActorSet menu = theTagList.GetObjectsTagged("menu");
  for ( Actor* actor : menu ) {
    actor->Destroy();
  }

  thePlayer = new Player::Monster(Augments::Weapon_Type::Big_Sword);
  theWorld.Add(thePlayer);
  thePlayer->SetAlpha(1.0f);

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
Game::Mouse* Game::theMouse = nullptr;

Game::Mouse::Mouse() {
  std::cout << "new mouse\n";
  mouse_position = &Vector2::Zero;
}

void Game::Mouse::MouseDownEvent(Vec2i screenCoord, MouseButtonInput button) {
  auto world_coord = MathUtil::ScreenToWorld(screenCoord);

  ActorSet buttons = theTagList.GetObjectsTagged("button");
  Vector2* current_position = theMouse->R_Mouse_Position();
  for ( Actor* button : buttons ) {
    if ( theMouse != nullptr && button->GetBoundingBox().Contains( MathUtil::ScreenToWorld(current_position->X, current_position->Y) ) ) {
      Game:theOverseer->pressed_button = button;
    }
  }
}

void Game::Mouse::MouseMotionEvent(Vec2i screenCoord) {
  // get border
  float true_width = utility::True_height*1.5f;
  float border = utility::True_width - true_width;

  mouse_position = new Vector2(screenCoord.X - border/2, screenCoord.Y);
}

Vector2* Game::Mouse::R_Mouse_Position() const {
  return mouse_position;
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
  menu_start->SetPosition(0,MathUtil::PixelsToWorldUnits(60));
  menu_controls->SetPosition(0,0);
  menu_exit->SetPosition(0,-MathUtil::PixelsToWorldUnits(60));
  menu_select->SetPosition(0,0);

  menu_start->SetSprite("Images\\menu_start.png");
  menu_controls->SetSprite("Images\\menu_controls.png");
  menu_exit->SetSprite("Images\\menu_exit.png");
  menu_select->SetSprite("Images\\menu_select.png");
  menu_select->SetAlpha(0.0f);

  theWorld.Add(menu_start);
  theWorld.Add(menu_controls);
  theWorld.Add(menu_exit);
  theWorld.Add(menu_select);
  menu_start->Tag("button, menu");
  menu_controls->Tag("button, menu");
  menu_exit->Tag("button, menu");
  menu_select->Tag("menu");
}

void Game::Overseer::Start_Game() {
  menu_select->Destroy();
  menu_exit->Destroy();
  menu_controls->Destroy();
  menu_start->Destroy();
  in_menu = 0;
}

void Game::Overseer::Update(float dt) {
  glfwGetWindowSize(theWorld.GetMainWindow(), &utility::True_width, &utility::True_height); // update true window width/height

  if ( in_menu ) {
    selected_icon = nullptr;

    ActorSet buttons = theTagList.GetObjectsTagged("button");
    Vector2* current_position = theMouse->R_Mouse_Position();
    for ( Actor* button : buttons ) {
      if ( theMouse != nullptr && button->GetBoundingBox().Contains( MathUtil::ScreenToWorld(current_position->X, current_position->Y) ) ) {
        selected_icon = button;
        break;
      }
    }

    if ( selected_icon == nullptr ) {
      menu_select->SetAlpha(0.0f);
    } else {
      menu_select->SetAlpha(1.0f);
      menu_select->SetPosition(selected_icon->GetPosition());
    }

    if ( pressed_button != nullptr ) {
      if ( pressed_button == menu_start ) {
        // start
        std::cout << "started\n";
        Game::Initialize_Game();
      } else if ( pressed_button == menu_controls ) {
        // controls
        std::cout << "controls\n";
       
      } else if ( pressed_button == menu_exit ) {
        // exit
        std::cout << "exited\n";
        theWorld.StopGame();
      }

      pressed_button = nullptr;
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