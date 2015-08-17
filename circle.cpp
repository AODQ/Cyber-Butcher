#include "stdafx.h"
#include "circle.h"
#include "utility.h"
#include "Angel.h"

#include <random>

Circle_Game_manager* Circle_Game_manager::circle_manager;
Circle_Game_manager& Circle_Game_manager::Get_Instance() {
  if ( !circle_manager )
    circle_manager = new Circle_Game_manager();
  return *circle_manager;
}
void Circle_Game_manager::Update ( float dx ) {

  // circles
  if ( circles.size() != 100 ) {
    Add_Circle(utility::R_Rand()-50,utility::R_Rand()-50);
  }

  // player

  static const float _Force = 2500;

  if ( theInput.IsKeyDown(GLFW_KEY_A) ) {
    player->ApplyForce(Vector2(-_Force, 0),Vector2(0,0));
  }
  if ( theInput.IsKeyDown(GLFW_KEY_W) ) {
    player->ApplyForce(Vector2(0 ,_Force),Vector2(0,0));
  }
  if ( theInput.IsKeyDown(GLFW_KEY_S) ) {
    player->ApplyForce(Vector2( 0, -_Force),Vector2(0,0));
  }
  if ( theInput.IsKeyDown(GLFW_KEY_D) ) {
    player->ApplyForce(Vector2(_Force, 0),Vector2(0,0));
  }
  if ( theInput.IsKeyDown(GLFW_KEY_Q) ) {
    player->ApplyTorque(15000.f);
  }

}

Circle::Circle() {
  vel_x = orig_vel_x = vel_y = orig_vel_y = 0.f;
  physics_trig = 0;
  max_lifetime = lifetime = (utility::R_Rand()/10 + 50)*25;
}
void Circle::Set_Velocity(float vx, float vy) {
  orig_vel_x = vel_x = vx;
  orig_vel_y = vel_y = vy;
}
float Circle::R_Velocity_X() { return vel_x; }
float Circle::R_Velocity_Y() { return vel_y; }
void Circle::Update(float dt) {
  SetAlpha(lifetime/max_lifetime);
  if ( physics_trig != 2 ) {
    SetPosition(GetPosition().X + vel_x*(dt/10),
                GetPosition().Y + vel_y*(dt/10));
    vel_x *= .996f;
    vel_y *= .996f;
    // initiate collapse
    if ( vel_x > -3 && vel_x < 3 &&
       vel_y > -3   && vel_y < 3 ) {
      vel_x = -orig_vel_x;
      vel_y = -orig_vel_y;
      if ( ++physics_trig == 2 ) {
        this->SetFriction(.8);
        this->SetDensity(GetSize().X/50.f);
        this->InitPhysics();
      }
    }
  } else if ( lifetime -- < 0 ) {
    for ( int i = 0; i < circles.size(); ++ i )
      if ( circles[i] == this ) {
        circles[i]->Destroy();
        circles.erase(circles.begin() + i, circles.begin() + i + 1);
      }
    
  }
}


const float Bullet::total_lifetime = 450;

Bullet::Bullet() {
  lifetime = total_lifetime;
  SetColor(.5f,.5f,.5f);
  SetDrawShape(ADS_Circle);
  SetSize(utility::R_Rand()/30);
  SetFriction(.2f);
  SetDensity(0.3f);
}
void Bullet::Update(float dt) {
  if ( lifetime-- < 0 ) {
    this->Destroy();
  }
}

const int Text_Fade::total_lifetime = 50;
Text_Fade::Text_Fade() {
  lifetime = total_lifetime;
}
void Text_Fade::Update(float dt) {
  SetPosition(Vector2(GetPosition().X,GetPosition().Y-dt*4.0f));
  SetAlpha(lifetime/total_lifetime);
  if ( --lifetime < 0 )
    this->Destroy();
}

void _Mouse::MouseDownEvent(Vec2i screenCoord, MouseButtonInput button ) {
  // create bullet
  auto coord = MathUtil::ScreenToWorld(screenCoord);
  float _ang = std::atan2f(coord.Y-player->GetPosition().Y,
                           coord.X-player->GetPosition().X);
  auto i = new Bullet();
  i->SetPosition(player->GetPosition().X+std::cos(_ang)*10,
                 player->GetPosition().Y+std::sin(_ang)*10);
  i->InitPhysics();
  i->GetBody()->SetGravityScale(16.f);
  theWorld.Add(i);
  float vx = std::cos(_ang)*600000,
        vy = std::sin(_ang)*600000;
  i->ApplyForce(Vector2(vx,vy),Vector2(0,0));
  i->ApplyForce(Vector2(vx,vy),Vector2(0,0));
  i->ApplyForce(Vector2(vx,vy),Vector2(0,0));
  utility::Output(
  "\nBullet x: " + std::to_string(i->GetPosition().X) + "\n" +
    "       y: " + std::to_string(i->GetPosition().Y) + "\n" +
    "   vel x: " + std::to_string(vx)      + "\n" +
    "   vel y: " + std::to_string(vy));
  Add_Fade_Text(std::to_string(vx+vy),i->GetPosition().X,
                                      i->GetPosition().Y);
}

bool Init_Game() {
  // world
  world.Initialize(utility::Window_height,
                   utility::Window_width, "Circle Test", true, false, true);
  theCamera.SetZByViewRadius(100);
  world.GetConsole()->WriteToOutput("Testing random output\n");
  for ( int i = 0; i != 5; ++ i )
    world.GetConsole()->WriteToOutput(std::to_string(utility::R_Rand()) + '\n');
  world.GetConsole()->WriteToOutput("Done\n");

  new _Mouse(); // initialize mouse event thingamajig
  
  world.SetGameManager(&Circle_Game_manager::Get_Instance());

  // physics
  world.SetupPhysics();

  GridActor* test_grid = new GridActor();
  test_grid->SetMaxCoord(Vector2(500, 500));
  test_grid->SetMinCoord(Vector2(-500, -500));
  test_grid->SetAxisColor(Color(0.0f, 0.0f, 0.0f));
  test_grid->SetLineColor(Color(0.9f, 0.9f, 0.9f));
  world.Add(test_grid);
  
  // background
  //FullScreenActor* backdrop = new FullScreenActor();
  //backdrop->SetSprite("LD33-Wallpaper1.png");
  //world.Add(backdrop);
  
  ParticleActor* pa = new ParticleActor();
  pa->SetColor(0.0f, 0.0f, 0.0f, 1.0f);
  pa->SetEndColor(Color(0.0f, 0.0f, 0.0f, 0.0f));
  pa->SetSize(Vector2(0.2f, 0.2f));
  pa->SetMaxParticles(5000);
  pa->SetParticlesPerSecond(1000.0f);
  pa->SetSpread(MathUtil::Pi);
  pa->SetSpeedRange(3.0f, 4.0f);
  pa->SetGravity(Vector2(0, -5));
  pa->SetParticleLifetime(5f);
  world.Add(pa);
  
  // ground
  auto t = new PhysicsActor();
  world.Add(t);
  t->SetPosition(-5,-40);
  t->SetColor(0,0,0);
  t->SetDrawShape(ADS_Square);
  t->SetSize(150,10);
  t->SetDensity(0);
  t->InitPhysics();
  //t->GetBody()->SetGravityScale(15.0f);

  player = new PhysicsActor();
  player->SetPosition(0,0);
  player->SetSprite("LD33-Wallpaper1.png");
  player->SetSize(10.f,10.f);
  player->InitPhysics();
  player->SetDensity(30.f);
  player->GetBody()->SetGravityScale(0.0f);
  world.Add(player);
  
  return 1;
}

void Add_Circle(int pos_x, int pos_y) {
  Circle* t_circle = new Circle();
  t_circle->SetPosition(pos_x, pos_y);
  t_circle->SetColor(utility::R_Rand()/100.f,
                     utility::R_Rand()/100.f,
                     utility::R_Rand()/100.f);
  t_circle->SetDrawShape(ADS_Circle);
  t_circle->SetSize(utility::R_Rand()/25.f + 1.f);
  t_circle->Set_Velocity((utility::R_Rand() - 50)*16,
                         (utility::R_Rand() - 50)*7);
  world.Add(t_circle);
  circles.push_back(t_circle);
}

void Add_Fade_Text(std::string text, int pos_x, int pos_y) {
  auto i = new Text_Fade();
  i->SetDisplayString(text);
  i->SetColor(Color(utility::rand()/100.f, // broken, contains only white values
                    utility::rand()/100.f,
                    utility::rand()/100.f));
  //i->SetRotation(utility::R_Rand()*3.6f);
  i->SetPosition(Vector2(pos_x,pos_y));
  theWorld.Add(i);
}


std::vector<Circle*> circles;
PhysicsActor* player;