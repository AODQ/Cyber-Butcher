#include "stdafx.h"
#include "Monster.h"
#include "Augments.h"
#include "Angel.h"
#include "Hero.h"
#include "Particle_System.h"

const int Player::Monster::idle_frame_max = 6;
const int Player::Monster::attack_frame_max = 8;
const int Player::Monster::walk_frame_max = 3;

void Player::Monster::Update(float dt) {

  chest_hitbox->GetBody()->SetTransform(b2Vec2(GetPosition().X, GetPosition().Y + .5f), 0.0f);

  // mirror according to direction
  if (direction == 1) {
    SetUVs(Vector2(0.f, 0.f), Vector2(1.f, 1.f));
  } else if (direction == 0) {
    SetUVs(Vector2(1.f, 0.f), Vector2(0.f, 1.f));
  }

  // jump
  if (phys_jump_timer > 0) {
    phys_jump_timer -= dt;
  }

  if ( theInput.IsKeyDown(GLFW_KEY_W) &&
      phys_jump_timer <= 0 ) {
    phys_jump_timer = theTuning.GetFloat("JumpTimer");
    ApplyForce(Vector2(0, theTuning.GetFloat("JumpVelocity")),Vector2(0,0));
  }

  float movement = 10;
  Vector2 target_velocity(movement, 0);
  b2Vec2 vel = GetBody()->GetLinearVelocity();
  float mass = GetBody()->GetMass();

  // movement
  if ( !is_attacking ) {
    if ( theInput.IsKeyDown(GLFW_KEY_D) ^
         theInput.IsKeyDown(GLFW_KEY_A) ) {
      if ( current_anim != Anim_Type::walk) {
        LoadSpriteFrames("Images\\monster_walk_001.png");
        anim_frame = 0;
      }
      
      current_anim = Anim_Type::walk;

      if ( theInput.IsKeyDown(GLFW_KEY_D) ) {
	      ApplyLinearImpulse(Vector2(mass*(target_velocity.X - vel.x)*dt*4, 0), Vector2(0, 0));
        direction = 0;
      }
      if ( theInput.IsKeyDown(GLFW_KEY_A) ) {
	      ApplyLinearImpulse(Vector2(mass*(-target_velocity.X - vel.x)*dt*4, 0), Vector2(0, 0));
        direction = 1;
      }
    } else {
      if ( !is_attacking ) {
        if ( current_anim != Anim_Type::idle )
          LoadSpriteFrames("Images\\monster_idle_001.png");
        current_anim = Anim_Type::idle;
      }
    }
  }

  if (Hero::theEnemy != nullptr &&
    Hero::theEnemy->GetBoundingBox().Intersects(GetBoundingBox()) &&
    GetBody()->GetLinearVelocity().y < 0 && !hit_ground_after_stomp ) {

    Hero::theEnemy->Add_Health(-stomp_damage);
    hit_ground_after_stomp = 1;
    Hero::theEnemy->ApplyForce(Vector2(0, 250), Vector2(0, 0));
    // cause bleed at top of middle of the enemy direction downwards
    Particles::Add_Bleed(Vec2i(Hero::theEnemy->GetPosition().X,
      Hero::theEnemy->GetPosition().Y-Hero::theEnemy->GetSize().Y/2),
      std::_Pi + std::_Pi/2);
  }

  if ( GetBody()->GetLinearVelocity().y == 0 )
      hit_ground_after_stomp = 0;

  if (GetBody()->GetLinearVelocity().y != 0 && !is_attacking) {
    if ( current_anim != Anim_Type::jump) {
      SetSprite("Images\\monster_jump_001.png");
    }
    current_anim = Anim_Type::jump;
    anim_frame = 0;
  }
  
  switch ( current_anim ) {
  case Anim_Type::idle:
    anim_frame += dt * 10 * (anim_direction?1:-1);
    if ( anim_direction && anim_frame >= idle_frame_max ) {
      anim_frame = idle_frame_max-1;
      anim_direction = 0;
    } else if ( !anim_direction && anim_frame < 0 ) {
      anim_frame = 0;
      anim_direction = 1;
    }
  break;
  case Anim_Type::attack:
    anim_frame += dt * 15;
    if ( anim_frame >= attack_frame_max ) {
      anim_frame = 0;
      anim_direction = 0;
      is_attacking = 0;
      current_anim = Anim_Type::idle;
      LoadSpriteFrames("Images\\monster_idle_001.png");
    }
    break;
  case Anim_Type::jump:
    anim_frame = 0;
  break;
  case Anim_Type::walk:
    anim_frame += dt * 5;
    if ( anim_frame >= walk_frame_max ) {
      anim_frame = 0;
    }
    break;
  default:
  break;
  }
  SetSpriteFrame(int(anim_frame));

  // while punching check at frame 7 for collision

  if ( current_anim == Anim_Type::attack &&
        int(anim_frame) == 7 ) {
    // create hitbox and check
    auto tz = new PhysicsActor;
    tz->SetPosition(GetPosition().X + (direction?-2:2),GetPosition().Y+0.85);
    tz->SetSize(MathUtil::PixelsToWorldUnits(35),
                MathUtil::PixelsToWorldUnits(15));
    tz->SetColor(.3,.3,.3);
    tz->SetIsSensor(1);
    tz->InitPhysics();
    tz->GetBody()->SetGravityScale(0);
    if ( tz->GetBoundingBox().Intersects(Hero::theEnemy->GetBoundingBox()) ) {
      // send that fucker FLYINGGGGGGGGGGGGGGG!!!!!!!!!!!!!!!!!!!!
      Hero::theEnemy->Add_Health(-attack_damage);
      Hero::theEnemy->ApplyForce(Vector2(direction?-1200:1200,0),Vector2(0,0));
      ++anim_frame;
      Particles::Add_Bleed(Vec2i(Hero::theEnemy->GetPosition().X,
                                 Hero::theEnemy->GetPosition().Y),
      direction?std::_Pi:0,1500);
      // recoil
      ApplyForce(Vector2(direction?-750:750,0),Vector2(0,0));
    }
    tz->Destroy();
  }
  
  // movement friction
  ApplyForce(Vector2(-GetBody()->GetLinearVelocity().x*dt*12,0),Vector2(0,0));

  if ( attack_cooldown >= 0 ) attack_cooldown -= dt;
  else if ( theInput.IsKeyDown(GLFW_KEY_J) && Hero::theEnemy ) {
    //frame_weapon->Cast();

    attack_cooldown = .7f;

    LoadSpriteFrames("Images\\monster_attack_001.png");
    current_anim = Anim_Type::attack;
    anim_frame = 0;
    is_attacking = true;
  }

  previous_direction = direction;
  previous_y_velocity = GetBody()->GetLinearVelocity().y;
};

int Player::Monster::R_Max_Health()    const { return max_health; }
int Player::Monster::R_Curr_Health()   const { return curr_health; }
int Player::Monster::R_Attack_Damage() const { return attack_damage; }
int Player::Monster::R_Stomp_Damage() const { return stomp_damage; }
bool Player::Monster::R_Direction()    const { return direction; }
PhysicsActor* Player::Monster::R_Chest_Hitbox() { return chest_hitbox; }

Augments::Weapon* Player::Monster::R_Frame_Weapon()  { return frame_weapon; }


void Player::Monster::Set_Max_Health(int x)  { max_health = x; }
void Player::Monster::Set_Curr_Health(int x) { curr_health = x; }
void Player::Monster::Add_Curr_Health(int x) { curr_health += x; }
void Player::Monster::Set_Attack_Damage(int x) { attack_damage = x; }
void Player::Monster::Set_Stomp_Damage(int x) { stomp_damage = x; }

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
  previous_direction = 0;
  anim_frame = 1;
  Set_Attack_Damage(12);
  Set_Stomp_Damage(3);
  SetColor(Color(1.0f, 1.0f, 1.0f));

  is_attacking = false;
  
  float anim_frame = 0;
  Anim_Type current_anim = Anim_Type::walk;
  LoadSpriteFrames("Images\\monster_idle_001.png");
  bool anim_direction = 0;
  // physics related
  this->SetFixedRotation(1);
  this->SetPosition(MathUtil::ScreenToWorld(100,185));
  SetSize(MathUtil::PixelsToWorldUnits(96),MathUtil::PixelsToWorldUnits(86));
  this->SetDrawSize(MathUtil::PixelsToWorldUnits(96),
                    MathUtil::PixelsToWorldUnits(96));
  InitPhysics();
  this->SetDensity(0.5f);
  // to avoid collision with hero
  auto fixture = GetBody()->GetFixtureList()->GetFilterData();
  fixture.groupIndex = -8;
  GetBody()->GetFixtureList()->SetFilterData(fixture);
  phys_jump_timer = 0;
  phys_jump_timer_max = 100000;

  chest_hitbox = new PhysicsActor();
  chest_hitbox->SetIsSensor(1);
  chest_hitbox->SetPosition(GetPosition());
  chest_hitbox->SetSize(MathUtil::PixelsToWorldUnits(40), MathUtil::PixelsToWorldUnits(30));
  chest_hitbox->SetColor(Color(1.0f, 0.f, 0.f));
  chest_hitbox->SetAlpha(0.0f);
  chest_hitbox->InitPhysics();
  chest_hitbox->GetBody()->SetGravityScale(0.0f);
  theWorld.Add(chest_hitbox, 5);
  chest_hitbox->GetBody()->SetTransform(b2Vec2(GetPosition().X, GetPosition().Y - 1), 0.0f);
}