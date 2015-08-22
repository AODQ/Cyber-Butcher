#include "stdafx.h"
#include "Monster.h"
#include "Augments.h"
#include "Angel.h"

void Player::Monster::Update(float dt) {
  dt = 900;
  if ( theInput.IsKeyDown(GLFW_KEY_W) )
    ApplyForce(Vec2i(0,-dt),Vec2i(0,0));
  if ( theInput.IsKeyDown(GLFW_KEY_D) )
    ApplyForce(Vec2i(dt, 0),Vec2i(0,0));
  if ( theInput.IsKeyDown(GLFW_KEY_A) )
    ApplyForce(Vec2i(-dt,0),Vec2i(0,0));
  if ( theInput.IsKeyDown(GLFW_KEY_S) )
    ApplyForce(Vec2i(0, dt),Vec2i(0,0));
    
};

int Player::Monster::R_Max_Health()    const { return max_health; }
int Player::Monster::R_Curr_Health()   const { return curr_health; }
int Player::Monster::R_Attack_Damage() const { return attack_damage; }

Augments::Head*   Player::Monster::R_Frame_Head()    { return frame_head; }
Augments::Body*   Player::Monster::R_Frame_Body()    { return frame_body; }
Augments::Weapon* Player::Monster::R_Frame_Weapon()  { return frame_weapon; }


void Player::Monster::Set_Max_Health(int x)  { max_health = x; }
void Player::Monster::Set_Curr_Health(int x) { curr_health = x; }
void Player::Monster::Add_Curr_Health(int x) { curr_health += x; }
void Player::Monster::Set_Attack_Damage(int x) { attack_damage = x; }

void Player::Monster::Set_Frame_Head(Augments::Head_Type x) {
  if ( frame_head != nullptr ) {
    frame_head->Destroy();
    delete frame_head;
    frame_head = nullptr;
  }
  frame_head = new Augments::Head(x);
  theWorld.Add(frame_head);
};
void Player::Monster::Set_Frame_Weapon(Augments::Weapon_Type x) {
  if ( frame_weapon != nullptr ) {
    theWorld.Remove(frame_weapon);
    delete frame_weapon;
    frame_weapon = nullptr;
  }
  frame_weapon = new Augments::Weapon(x);
  theWorld.Add(frame_weapon);
};
void Player::Monster::Set_Frame_Body(Augments::Body_Type x) {
  if ( frame_body != nullptr ) {
    theWorld.Remove(frame_body);
    delete frame_body;
    frame_body = nullptr;
  }
  frame_body = new Augments::Body(x);
  theWorld.Add(frame_body);
};
  
  
Player::Monster::Monster(Augments::Head_Type head,
                  Augments::Weapon_Type weapon,
                  Augments::Body_Type body) {
  frame_body = nullptr;
  frame_head = nullptr;
  frame_weapon = nullptr;
  Set_Frame_Body(body);
  Set_Frame_Head(head);
  Set_Frame_Weapon(weapon);
  
  this->SetFixedRotation(0);
  this->SetDensity(0.01);
  
  InitPhysics();
}