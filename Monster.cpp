#include "stdafx.h"
#include "Monster.h"
#include "Augments.h"
#include "Angel.h"
#include "Hero.h"

const int Player::Monster::idle_frame_max = 7;

void Player::Monster::Update(float dt) {
  // jump
  phys_jump_timer -= dt;
  if ( theInput.IsKeyDown(GLFW_KEY_W) &&
      phys_jump_timer < 0 ) {
    phys_jump_timer = theTuning.GetFloat("JumpTimer");
    ApplyForce(Vec2i(0, theTuning.GetFloat("JumpVelocity")),Vec2i(0,0));
  }

  float movement = 8;
  Vector2 target_velocity(movement, 0);
  b2Vec2 vel = GetBody()->GetLinearVelocity();
  float mass = GetBody()->GetMass();
  

  // movement
  if ( theInput.IsKeyDown(GLFW_KEY_D) ^
       theInput.IsKeyDown(GLFW_KEY_A) ) {
    if ( theInput.IsKeyDown(GLFW_KEY_D) ) {
	    ApplyLinearImpulse(Vector2((target_velocity.X - vel.x), 0), Vector2(0, 0));
      direction = 0;
    }
    if ( theInput.IsKeyDown(GLFW_KEY_A) ) {
	    ApplyLinearImpulse(Vector2((-target_velocity.X - vel.x), 0), Vector2(0, 0));
      direction = 1;
    }
  } else {
    if ( current_anim != Anim_Type::idle )
      LoadSpriteFrames("Images\\monster_idle_001.png");
    current_anim = Anim_Type::idle;
  }
  anim_frame += dt * 2 * (anim_direction?1:-1);
  if ( anim_direction && anim_frame >= idle_frame_max ) {
    anim_frame = idle_frame_max-1;
    anim_direction = 0;
  } else if ( !anim_direction && anim_frame < 0 ) {
    anim_frame = 1;
    anim_direction = 1;
  }
  SetSpriteFrame(anim_frame);
  


  // movement friction
  ApplyForce(Vector2(-GetBody()->GetLinearVelocity().x*.7,0),Vector2(0,0));

  if ( attack_cooldown >= 0 ) attack_cooldown -= dt;
  else if ( theInput.IsKeyDown(GLFW_KEY_J) && Hero::theEnemy ) {
    frame_weapon->Cast();
    Hero::theEnemy->Add_Health(-1000);
    attack_cooldown = [&]()->float{
      switch( R_Frame_Weapon()->R_Type() ) {
        case Augments::Weapon_Type::Big_Sword:
          return theTuning.GetInt("BigSwordAttackCooldown");
      }
      return 2.0f;
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
  inair = 1;
  frame_weapon = nullptr;
  Set_Frame_Weapon(weapon);
  SetFriction(1);
  direction = 0;
  
  float anim_frame = 0;
  Anim_Type current_anim = Anim_Type::walk;
  LoadSpriteFrames("Images\\monster_idle_001.png");
  bool anim_direction = 0;
  // physics related
  this->SetFixedRotation(1);
  this->SetPosition(MathUtil::ScreenToWorld(100,185));
  SetSize(MathUtil::PixelsToWorldUnits(40),MathUtil::PixelsToWorldUnits(86));
  this->SetDrawSize(MathUtil::PixelsToWorldUnits(83),
                    MathUtil::PixelsToWorldUnits(86));
  InitPhysics();
  this->SetDensity(0.5f);
  // to avoid collision with hero
  auto fixture = GetBody()->GetFixtureList()->GetFilterData();
  fixture.groupIndex = -8;
  GetBody()->GetFixtureList()->SetFilterData(fixture);
  phys_jump_timer = 0;
  phys_jump_timer_max = 100000;
}