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
  if ( circles.size() != 200 ) {
    Add_Circle(utility::R_Rand()-50,utility::R_Rand()-50);
  }

  // player

  static const float _Force = 250;

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
    player->ApplyTorque(1500.f);
  }

}

Circle::Circle() {
  vel_x = orig_vel_x = vel_y = orig_vel_y = 0.f;
  physics_trig = 0;
  max_lifetime = lifetime = (utility::R_Rand()/10 + 50)*50;
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
  } else if ( lifetime-- < 0 ) {
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
  float vx = std::cos(_ang)*60000000,
        vy = std::sin(_ang)*60000000;
  i->ApplyForce(Vector2(vx,vy),Vector2(0,0));
  i->ApplyForce(Vector2(vx,vy),Vector2(0,0));
  i->ApplyForce(Vector2(vx,vy),Vector2(0,0));
  theWorld.GetConsole()->WriteToOutput(
  "\nBullet x: " + std::to_string(i->GetPosition().X) + "\n" +
    "       y: " + std::to_string(i->GetPosition().Y) + "\n" +
    "   vel x: " + std::to_string(vx)      + "\n" +
    "   vel y: " + std::to_string(vy));
}

bool Init_Game() {
  // world
  world.Initialize(utility::Window_height,
                   utility::Window_width, "Circle Test", true, false);
  theCamera.SetZByViewRadius(100);
  world.GetConsole()->WriteToOutput("Testing random output\n");
  for ( int i = 0; i != 5; ++ i )
    world.GetConsole()->WriteToOutput(std::to_string(utility::R_Rand()) + '\n');
  world.GetConsole()->WriteToOutput("Done\n");

  new _Mouse(); // initialize mouse event thingamajig

  // physics
  world.SetupPhysics();

  world.Add(new GridActor());
  world.SetGameManager(&Circle_Game_manager::Get_Instance());

  auto t = new PhysicsActor();
  world.Add(t);
  t->SetPosition(-5,-40);
  t->SetColor(0,0,0);
  t->SetDrawShape(ADS_Square);
  t->SetSize(150,10);
  t->SetDensity(0);
  t->InitPhysics();
  t->GetBody()->SetGravityScale(0.0f);

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



std::vector<Circle*> circles;
PhysicsActor* player;