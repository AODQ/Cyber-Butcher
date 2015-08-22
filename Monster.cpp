#include "stdafx.h"
#include "Monster.h"
#include "Augments.h"
#include "Angel.h"

void Player::Monster::Update(float dt) {
  dt *= 7000;
  if ( phys_jump_timer >= 0 ) phys_jump_timer -= dt;
  // jump
  if ( theInput.IsKeyDown(GLFW_KEY_W) &&
      phys_jump_timer < 0 ) {
    ApplyForce(Vec2i(0, theTuning.GetFloat("JumpVelocity")),Vec2i(0,0));
    phys_jump_timer = theTuning.GetInt("JumpTimer"); 
  }
  // movement
  if ( theInput.IsKeyDown(GLFW_KEY_D) )
    ApplyForce(Vec2i(dt, 0),Vec2i(0,0));
  if ( theInput.IsKeyDown(GLFW_KEY_A) )
    ApplyForce(Vec2i(-dt,0),Vec2i(0,0));
   
  // movement friction
  ApplyForce(Vector2(-GetBody()->GetLinearVelocity().x/5,0),Vector2(0,0));
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
  if ( !SetSprite("Images\\body.png") ) {
    int x = 0;
    x = 5;
    SetSprite("head.png");
  }

  // physics related
  this->SetFixedRotation(1);
  this->SetPosition(0,0);
  InitPhysics();
  this->SetPosition(0,0);
  this->SetDensity(1.0f);
  // to avoid collision with hero
  auto fixture = GetBody()->GetFixtureList()->GetFilterData();
  fixture.groupIndex = -8;
  GetBody()->GetFixtureList()->SetFilterData(fixture);
  phys_jump_timer = 0;
  phys_jump_timer_max = 100000;
}