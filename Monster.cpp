#include "stdafx.h"
#include "Monster.h"
#include "Augments.h"
#include "Angel.h"

void Player::Monster::Update(float dt) {
  dt *= 300;
  if ( phys_jump_timer >= 0 ) phys_jump_timer -= dt;
  // jump
  if ( theInput.IsKeyDown(GLFW_KEY_W) &&
      phys_jump_timer < 0 ) {
    ApplyLinearImpulse(Vector2(0, 50), Vector2(0, 0));
    //ApplyForce(Vec2i(0, theTuning.GetFloat("JumpVelocity")),Vec2i(0,0));
    phys_jump_timer = theTuning.GetInt("JumpTimer"); 
  }

  Vector2 target_velocity(dt, 0);
  b2Vec2 vel = GetBody()->GetLinearVelocity();
  float mass = GetBody()->GetMass();

  // movement
  if ( theInput.IsKeyDown(GLFW_KEY_D) ^
       theInput.IsKeyDown(GLFW_KEY_A) ) {
    if ( theInput.IsKeyDown(GLFW_KEY_D) ) {
	    ApplyLinearImpulse(Vector2(mass * target_velocity.X - vel.x, 0), Vector2(0, 0));
      direction = 0;
    }
    if ( theInput.IsKeyDown(GLFW_KEY_A) ) {
	    ApplyLinearImpulse(Vector2(mass * -target_velocity.X - vel.x, 0), Vector2(0, 0));
      direction = 1;
    }
  }
   
  // movement friction
  ApplyForce(Vector2(-GetBody()->GetLinearVelocity().x/.4),Vector2(0,0));

  if ( attack_cooldown >= 0 ) attack_cooldown -= dt;
  else 
  if ( theInput.IsKeyDown(GLFW_KEY_J) ) {
    frame_weapon->Cast();
    attack_cooldown = 7000*[&]()->float{
      switch( R_Frame_Weapon()->R_Type() ) {
        case Augments::Weapon_Type::Big_Sword:
          return theTuning.GetInt("BigSwordAttackCooldown");
      }
      return 250.0f;
    }();
  }
};

int Player::Monster::R_Max_Health()    const { return max_health; }
int Player::Monster::R_Curr_Health()   const { return curr_health; }
int Player::Monster::R_Attack_Damage() const { return attack_damage; }
bool Player::Monster::R_Direction()    const { return direction; }

Augments::Weapon* Player::Monster::R_Frame_Weapon()  { return frame_weapon; }


void Player::Monster::Set_Max_Health(int x)  { max_health = x; }
void Player::Monster::Set_Curr_Health(int x) { curr_health = x; }
void Player::Monster::Add_Curr_Health(int x) { curr_health += x; }
void Player::Monster::Set_Attack_Damage(int x) { attack_damage = x; }

void Player::Monster::Set_Frame_Weapon(Augments::Weapon_Type x) {
  if ( frame_weapon != nullptr ) {
    theWorld.Remove(frame_weapon);
    delete frame_weapon;
    frame_weapon = nullptr;
  }
  frame_weapon = new Augments::Weapon(x);
  theWorld.Add(frame_weapon);
  //frame_weapon->SetLayer(Game::thePlayer->GetLayer()+1);
};
  
  
Player::Monster::Monster(Augments::Weapon_Type weapon) {
  frame_weapon = nullptr;
  Set_Frame_Weapon(weapon);

  direction = 0;

  // physics related
  this->SetFixedRotation(1);
  this->SetPosition(MathUtil::ScreenToWorld(100,185));
  this->SetSize(MathUtil::PixelsToWorldUnits(16),
                MathUtil::PixelsToWorldUnits(32));
  this->SetDrawShape(ADS_Square);
  SetColor(0.3,0.2,0.6);
  InitPhysics();
  this->SetDensity(0.5f);
  // to avoid collision with hero
  auto fixture = GetBody()->GetFixtureList()->GetFilterData();
  fixture.groupIndex = -8;
  GetBody()->GetFixtureList()->SetFilterData(fixture);
  phys_jump_timer = 0;
  phys_jump_timer_max = 100000;
  // set user data
  GetBody()->SetUserData(this);
}