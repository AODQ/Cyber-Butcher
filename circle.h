#ifndef __CIRCLE_H_
#define __CIRCLE_H_

#include "stdafx.h"
#include "Angel.h"

#define world World::GetInstance()
#define camera Camera::GetInstance()

// game manager handles mainly just calling update and screen refresh
class Circle_Game_manager : public GameManager {
  static Circle_Game_manager* circle_manager;
public:
  static Circle_Game_manager& Get_Instance();
  void Update(float dt);
};

// main object for game. Just a simple circle with a velocity that
// initiates an explosion and collapse
class Circle : public PhysicsActor {
  float vel_x, vel_y, orig_vel_x, orig_vel_y;
  int physics_trig;
  float lifetime, max_lifetime;
public:
  Circle();
  void Set_Velocity(float vx, float vy);
  float R_Velocity_X();
  float R_Velocity_Y();
  void Update(float dt);
};

class Bullet : public PhysicsActor {
  float lifetime;
  static const float total_lifetime;
public:
  Bullet();
  void Update(float dt);
};

class _Mouse : public MouseListener {
public:
  void MouseDownEvent(Vec2i screenCoord, MouseButtonInput button );
};

class Text_Fade : public TextActor {
  float lifetime;
  static const int total_lifetime;
public:
  Text_Fade();
  void Update(float);
};

bool Init_Game();
void Add_Circle(int posx, int posy);
void Add_Fade_Text(std::string text, int, int);

extern std::vector<Circle*> circles;
extern PhysicsActor* player;
extern ParticleActor* pa;

#endif