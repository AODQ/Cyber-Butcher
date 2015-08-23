#include "stdafx.h"
#include "Hero.h"
#include "Angel.h"
#include "utility.h"
#include "Monster.h"
#include "LD33.h"

Hero::Enemy::Enemy() {
  theEnemy = this;
  SetPosition(Vector2(-14.5f, -1.5625));
  SetDrawShape(ADS_Square);
  SetSize(1,1);
  SetColor(.2,.2,.2);
  SetFixedRotation(true);

  InitPhysics();
  // avoid contact with player
  auto fixture = GetBody()->GetFixtureList()->GetFilterData();
  fixture.groupIndex = -8;
  GetBody()->GetFixtureList()->SetFilterData(fixture);
  // set user data
  GetBody()->SetUserData(this);

  intro = true;
  speed = 3;
  health = 5;
};

void Hero::Enemy::Update(float dt) {
  int direction = 1;
  float distance_x = GetPosition().X - Game::thePlayer->GetPosition().X,
        distance_y = GetPosition().Y - Game::thePlayer->GetPosition().Y;

  melee_cooldown -= dt;
  range_cooldown -= dt;
  movement_cooldown -= dt;
  movement_attack_flinch -= dt;
  select_ability_timer -= dt;

  mood_switch_timer -= dt;

  if ( mood_switch_timer <= 0 ) {
    mood = (Mood)(int(utility::R_Rand())%int(Mood::Size));
    mood_switch_timer = 20;
  }

  // performing abilities

  if ( ghost_timer > 0 ) {
    ghost_timer -= dt;
    // fade to alpha
    float alpha = (ghost_timer-4)/4;
    SetAlpha(alpha<.2?.2:alpha);
    if ( ghost_timer < 0 )
      SetAlpha(1);
  }

  if ( slide_timer > 0 ) {
    slide_timer -= dt;
    ApplyForce(Vector2(slide_direction*dt,0),0);
    return; // can't attack or move
  }

  if ( jumping_to_platform > 0 ) {
    if ( GetPosition().Y > 2 )  {
      jumping_to_platform = 0;
      platform = new Level::Platform();
      on_platform_timer = utility::R_Rand()/20;
    }
    return; // can't attack or move
  }

  if ( on_platform_timer > 0 ) {
    on_platform_timer -= dt;
    if ( on_platform_timer <= 0 ) {
      platform->Destroy();
    }
    goto SKIP_MOVEMENT_PHASE;
  }

  // decide to move
  switch ( mood ) {
    case Mood::Close: // up close and personal
      if ( abs(distance_x) < 1 ) { // perfect
        break;
      }
      if ( abs(distance_x) < 3 ) { // I can inch up a bit closer
        ApplyForce(Vector2(distance_x*dt * .4,0),Vector2(0,0));
        break;
      }
      // I'm too far away.
      ApplyForce(Vector2(distance_x*dt * 2,0),Vector2(0,0));
    break;
    case Mood::Fair: // stab and throw
      if ( abs(distance_x) < 1 ) { // Back up a bit
        ApplyForce(Vector2(distance_x*dt * .6),Vector2(0,0));
        break;
      }
      if ( abs(distance_x) < 3 ) { // perfect
        break;
      }
      if ( abs(distance_x < 6 ) ) { // I can walk up
        ApplyForce(Vector2(distance_x*dt * .5),Vector2(0,0));
        break;
      }
      // I'm too far away
      ApplyForce(Vector2(distance_x*dt * 4,0),Vector2(0,0));
    break;
    case Mood::Distant:
      if ( abs(distance_x) < 3 ) { // I'm too close
        ApplyForce(Vector2(distance_x*dt * 4.0),Vector2(0,0));
        break;
      }
      if ( abs(distance_x) < 6 ) { // back up
        ApplyForce(Vector2(distance_x*dt * 2.0),Vector2(0,0));
        break;
      }
      // I'm at a good distance
    break;
  }

  jump_timer -= dt;

  if ( jump_timer < 0 ) {
    jump_timer = 3;
    ApplyForce(Vec2i(0,-15),Vec2i(0,0));
  }

  SKIP_MOVEMENT_PHASE:;

  // attacks options
  if ( movement_attack_flinch > 0 ) return; // hero just attacked
  if ( melee_cooldown < 0 && abs(distance_x) <= 2 ) {
    Attack_Melee();
    return;
  }
  if ( range_cooldown < 0 && abs(distance_x) > 2 ) {
    Attack_Range();
    return;
  }

  // ability options
  
  if ( select_ability_timer < 0 && utility::R_Rand() < 10 ) {
    select_ability_timer = 1;
    // slide
    if ( distance_x < 5 && utility::R_Rand() < 25 ) {
      slide_timer = 2;
      slide_direction = (distance_x < 0);
    }
    
    // ghost
    if ( utility::R_Rand() < 5 && ghost_timer > 0 ) {
      ghost_timer = 5;
    }

    // jump to platform and player is under one of the platforms
    if ( utility::R_Rand() < 20  &&
          ( GetPosition().X > -10 && GetPosition().X < -8 ) ||
          ( GetPosition().X <  10 && GetPosition().X >  8 ) ) {
      jumping_to_platform = 1;
      return;
    }
  }
}

void Hero::Enemy::Jump() {
  
}

void Hero::Enemy::Attack_Melee() {

}

void Hero::Enemy::Attack_Range() {

}

void Hero::Enemy::Killed() {
  // particle effects when killed
  float lifetime = .5f;
  ParticleActor* killed_effect = new ParticleActor();
  killed_effect->SetPosition(this->GetPosition());
  killed_effect->SetParticleLifetime(lifetime);
  killed_effect->SetSystemLifetime(lifetime);
  killed_effect->SetMaxParticles(200);
  killed_effect->SetParticlesPerSecond(3000);
  killed_effect->SetSpread(3.14159265358979f);
  killed_effect->SetMinSpeed(.5f);
  killed_effect->SetMaxSpeed(5.0f);
  killed_effect->SetSize(Vector2(.1f, .1f));
  killed_effect->SetGravity(Vector2(0, .15f));
  killed_effect->SetColor(Color(1.0f, 0.72549f, 0.0f, 1.0f));
  killed_effect->SetEndColor(Color(.4f, .4f, .4f, 0.0f));
  theWorld.Add(killed_effect);

  for ( int i = 0; i != 15; ++ i ) {
    Hero::Gold* dropped_gold = new Hero::Gold(this->GetPosition());
    theWorld.Add(dropped_gold);
    dropped_gold->ApplyForce(Vector2((std::rand()-50)/20,std::rand()/25),Vector2(0,0));
  }

  this->Destroy();
  Game::theOverseer->level++;
  theEnemy = nullptr;
  Game::theKeep->time_left = 10.0f;
  Game::theKeep->NewItems();
}

Hero::Gold::Gold(Vector2 pos) {
  //SetIsSensor(true);
  SetDrawShape(ADS_Square);
  SetPosition(pos);
  SetDensity(.1f);
  SetSize(.5f, .5f);
  SetColor(Color(1.0f, 0.72549f, 0.0f));

  InitPhysics();
  
  // avoid contact from player
  auto fixture = GetBody()->GetFixtureList()->GetFilterData();
  fixture.groupIndex = -8;
  GetBody()->GetFixtureList()->SetFilterData(fixture);
  // set user data
  GetBody()->SetUserData(this);
}

void Hero::Enemy_Listener::ReceiveMessage(Message* m) {
  if ( m->GetMessageName() == std::string("FinishedHeroMovement") ) {
    theEnemy = new Enemy();
    theWorld.Add(theEnemy);
    //theEnemyIntro->Destroy();
    //theEnemyIntro = nullptr;
  }
}

void Hero::Gold::Update(float dt) {
  if ( this->GetBoundingBox().Intersects(Game::thePlayer->GetBoundingBox()) ) {
    Game::thePlayer->Add_Gold(utility::R_Rand()/25);
    Destroy();
  }  
}


Hero::Enemy* Hero::theEnemy = nullptr;
Hero::Enemy_Listener* Hero::e_listener = nullptr;
//Hero::Enemy_Intro* Hero::theEnemyIntro = nullptr;


// enemy attacks

Hero::Dagger::Dagger(float angl, Vec2i pos) {
  SetPosition(pos);
  SetRotation(angl);
  SetFixedRotation(angl);
  //SetSize(MathUtil::ScreenToWorld(Vec2i(100,0)).X,
  //        MathUtil::ScreenToWorld(Vec2i(0, 20)).Y);
  SetSprite("Images\\Knive.png");
  SetDensity(0.01f);
  this->SetRestitution(.6f);
  SetIsSensor(1);
  InitPhysics();
  //this->GetBody()->SetTransform(GetBody()->GetPosition(), angl );
  ApplyForce(Vec2i(std::cos(angl)*theTuning.GetFloat("DaggerForce"),
                   std::sin(angl)*theTuning.GetFloat("DaggerForce")),
                   Vector2(0,0));
  lifetime = theTuning.GetInt("DaggerLifetime");
  // set user data
  GetBody()->SetUserData(this);
}

void Hero::Dagger::Update(float dt) {
  lifetime -= dt;
  if ( lifetime < 0 ) {
    Destroy();
  }

  if ( GetBoundingBox().Intersects(Game::thePlayer->GetBoundingBox()) ) {
    Destroy();
  }

}