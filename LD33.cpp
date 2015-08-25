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
#include <string>

TextActor* gold;

// input key presses
int Game::right_key = 65;
int Game::left_key = 68;
int Game::punch_key = 74;
int Game::jump_key = 87;

bool Game::in_menu = 0;
bool Game::controls_open = false;
bool Game::input_polling = false;

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
  theOverseer->SetAlpha(0);
  theWorld.SetBackgroundColor(Color(0.0117647f, 0.1098039f, 0.04313725f));

  Game::theMouse = new Mouse();

  // collision
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
  Level::_HUD::Init();
}

Player::Monster* Game::thePlayer = nullptr;

Augments::ShopKeep* Game::theKeep = nullptr;
Game::Overseer* Game::theOverseer = nullptr;
Game::Mouse* Game::theMouse = nullptr;

Game::Mouse::Mouse() {
  mouse_position = &Vector2::Zero;
}

static void InputCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  std::cout << "inputted key: " << key << '\n';
  if ( Game::input_polling && action == 1 ) {
    Game::input_polling = false;
    
    Game::theOverseer->ChangeKey(key);
  }
}

void Game::Overseer::ChangeKey(int key) {
  if ( Game::Overseer::change_key == Game::Overseer::right_key ) {
    Game::right_key = key;
    std::cout << "changed right_key to " << key << '\n';
  } else if ( Game::Overseer::change_key == Game::Overseer::left_key ) {
    Game::left_key = key;
  } else if ( Game::Overseer::change_key == Game::Overseer::punch_key ) {
    Game::punch_key = key;
  } else if ( Game::Overseer::change_key == Game::Overseer::jump_key ) {
    Game::jump_key = key;
  }
}

void Game::Mouse::MouseDownEvent(Vec2i screenCoord, MouseButtonInput button) {
  auto world_coord = MathUtil::ScreenToWorld(screenCoord);

  ActorSet buttons = theTagList.GetObjectsTagged("button");
  Vector2* current_position = theMouse->R_Mouse_Position();
  for ( Actor* button : buttons ) {
    if ( theMouse != nullptr && button->GetBoundingBox().Contains( MathUtil::ScreenToWorld(current_position->X, current_position->Y)) && button->GetAlpha() > 0.f ) {
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

  // base menu
  title_logo = new Actor();
  menu_background = new Actor();
  menu_start = new Actor();
  menu_controls = new Actor();
  menu_exit = new Actor();

  // control menu
  right_key = new Actor();
  left_key = new Actor();
  punch_key = new Actor();
  jump_key = new Actor();
  back_arrow = new Actor();
<<<<<<< HEAD
=======
  right_text = new TextActor();
  left_text = new TextActor();
  punch_text = new TextActor();
  jump_text = new TextActor();
>>>>>>> 21508a30f2249ec0bea47e3bf4cb228d0095bb46

  title_logo->SetSize(MathUtil::PixelsToWorldUnits(224), MathUtil::PixelsToWorldUnits(102));
  menu_background->SetSize(MathUtil::PixelsToWorldUnits(224), MathUtil::PixelsToWorldUnits(128));
  menu_start->SetSize(MathUtil::PixelsToWorldUnits(128), MathUtil::PixelsToWorldUnits(32));
  menu_controls->SetSize(MathUtil::PixelsToWorldUnits(128), MathUtil::PixelsToWorldUnits(32));
  menu_exit->SetSize(MathUtil::PixelsToWorldUnits(128), MathUtil::PixelsToWorldUnits(32));

  right_key->SetSize(MathUtil::PixelsToWorldUnits(68), MathUtil::PixelsToWorldUnits(10));
  left_key->SetSize(MathUtil::PixelsToWorldUnits(68), MathUtil::PixelsToWorldUnits(10));
  punch_key->SetSize(MathUtil::PixelsToWorldUnits(68), MathUtil::PixelsToWorldUnits(10));
  jump_key->SetSize(MathUtil::PixelsToWorldUnits(68), MathUtil::PixelsToWorldUnits(10));
  back_arrow->SetSize(MathUtil::PixelsToWorldUnits(68), MathUtil::PixelsToWorldUnits(13));

  title_logo->SetPosition(0, MathUtil::PixelsToWorldUnits(85));
  menu_background->SetPosition(0, -MathUtil::PixelsToWorldUnits(55));
  menu_start->SetPosition(0,-MathUtil::PixelsToWorldUnits(20));
  menu_controls->SetPosition(0,-MathUtil::PixelsToWorldUnits(55));
  menu_exit->SetPosition(0,-MathUtil::PixelsToWorldUnits(90));

  right_key->SetPosition(MathUtil::PixelsToWorldUnits(35), -MathUtil::PixelsToWorldUnits(21));
  left_key->SetPosition(MathUtil::PixelsToWorldUnits(35), -MathUtil::PixelsToWorldUnits(36));
  punch_key->SetPosition(MathUtil::PixelsToWorldUnits(35), -MathUtil::PixelsToWorldUnits(49));
  jump_key->SetPosition(MathUtil::PixelsToWorldUnits(35), -MathUtil::PixelsToWorldUnits(63));
  back_arrow->SetPosition(MathUtil::PixelsToWorldUnits(35), -MathUtil::PixelsToWorldUnits(77));
<<<<<<< HEAD
=======
  right_text->SetPosition(-MathUtil::PixelsToWorldUnits(30), -MathUtil::PixelsToWorldUnits(25));
  left_text->SetPosition(-MathUtil::PixelsToWorldUnits(30), -MathUtil::PixelsToWorldUnits(39));
  punch_text->SetPosition(-MathUtil::PixelsToWorldUnits(30), -MathUtil::PixelsToWorldUnits(53));
  jump_text->SetPosition(-MathUtil::PixelsToWorldUnits(30), -MathUtil::PixelsToWorldUnits(67));
>>>>>>> 21508a30f2249ec0bea47e3bf4cb228d0095bb46

  title_logo->SetSprite("Images\\title_logo.png");
  menu_background->SetSprite("Images\\menu_background.png");
  menu_start->LoadSpriteFrames("Images\\start_001.png");
  menu_controls->LoadSpriteFrames("Images\\controls_001.png");
  menu_exit->LoadSpriteFrames("Images\\exit_001.png");

  right_key->LoadSpriteFrames("Images\\right_key_001.png");
  left_key->LoadSpriteFrames("Images\\left_key_001.png");
  punch_key->LoadSpriteFrames("Images\\punch_key_001.png");
  jump_key->LoadSpriteFrames("Images\\jump_key_001.png");
  back_arrow->LoadSpriteFrames("Images\\back_arrow_001.png");

  theWorld.Add(title_logo);
  theWorld.Add(menu_background);
  theWorld.Add(menu_start);
  theWorld.Add(menu_controls);
  theWorld.Add(menu_exit);

  theWorld.Add(right_key);
  theWorld.Add(left_key);
  theWorld.Add(punch_key);
  theWorld.Add(jump_key);
  theWorld.Add(back_arrow);
<<<<<<< HEAD
=======
  theWorld.Add(right_text);
  theWorld.Add(left_text);
  theWorld.Add(punch_text);
  theWorld.Add(jump_text);
>>>>>>> 21508a30f2249ec0bea47e3bf4cb228d0095bb46

  title_logo->Tag("menu");
  menu_background->Tag("menu");
  menu_start->Tag("button, menu, menu_button");
  menu_controls->Tag("button, menu, menu_button");
  menu_exit->Tag("button, menu, menu_button");

  right_key->Tag("button, menu, control_button");
  left_key->Tag("button, menu, control_button");
  punch_key->Tag("button, menu, control_button");
  jump_key->Tag("button, menu, control_button");
  back_arrow->Tag("button, menu, control_button");

<<<<<<< HEAD
=======
  right_text->Tag("menu, control_button");
  left_text->Tag("menu, control_button");
  punch_text->Tag("menu, control_button");
  jump_text->Tag("menu, control_button");

>>>>>>> 21508a30f2249ec0bea47e3bf4cb228d0095bb46
  glfwSetKeyCallback(theWorld.GetMainWindow(), InputCallback);
}

void Game::Overseer::Start_Game() {
  ActorSet menu = theTagList.GetObjectsTagged("button");
  for ( Actor* menu_actor : menu ) {
    menu_actor->Destroy();
    delete menu_actor;
  }
  in_menu = 0;
}

void Game::Overseer::Update(float dt) {
  glfwGetWindowSize(theWorld.GetMainWindow(), &utility::True_width, &utility::True_height); // update true window width/height

  if ( in_menu ) {
    ActorSet buttons = theTagList.GetObjectsTagged("button");
    Vector2* current_position = theMouse->R_Mouse_Position();
    for ( Actor* button : buttons ) {
      if ( theMouse != nullptr && button->GetBoundingBox().Contains( MathUtil::ScreenToWorld(current_position->X, current_position->Y) ) ) {
        button->SetSpriteFrame(1);
      } else {
        button->SetSpriteFrame(0);
      }
    }

    ActorSet menus = theTagList.GetObjectsTagged("menu_button");
    for ( Actor* actor : menus ) {
      if ( Game::controls_open ) {
        actor->SetAlpha(0.f);
      } else {
        actor->SetAlpha(1.f);
      }
    }

    ActorSet controls = theTagList.GetObjectsTagged("control_button");
    for ( Actor* actor : controls ) {
      if ( !Game::controls_open ) {
        actor->SetAlpha(0.f);
      } else {
        actor->SetAlpha(1.f);
      }
    }

    if ( pressed_button != nullptr ) {
      if ( !controls_open ) {
        if ( pressed_button == menu_start ) {
          // start
          Game::Initialize_Game();
        } else if ( pressed_button == menu_controls ) {
          // controls
          menu_background->SetSprite("Images\\menu_background_controls.png");
          controls_open = true;
        } else if ( pressed_button == menu_exit ) {
          // exit
          theWorld.StopGame();
        }
      } else {
        if ( pressed_button == back_arrow ) {
          menu_background->SetSprite("Images\\menu_background.png");
          controls_open = false;
        } else { 
          input_polling = true;
          change_key = pressed_button;
        }
      }
      pressed_button = nullptr;
    }

  } else {

    if (Hero::theEnemy == nullptr && theKeep->time_left <= 0 ) {
      std::cout << "New Hero\n";
      Hero::theEnemy = new Hero::Enemy();
      Hero::theEnemy->Add_Health(level*20);
      theWorld.Add(Hero::theEnemy);
      Game::thePlayer->Add_Curr_Health(level*2>12?12:level*2);
    }

    Particles::Update(dt);
    Level::BG_Scroll::Update(dt);
    Level::Leaves::Update(dt);

    Level::_HUD::Apply();
    Sounds::Update(dt);
  }
}