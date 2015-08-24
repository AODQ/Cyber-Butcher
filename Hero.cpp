#include "stdafx.h"
#include "Hero.h"
#include "Angel.h"
#include "utility.h"
#include "Monster.h"
#include "Particle_System.h"
#include "LD33.h"
#include "Sounds.h"

Hero::E_Weapon::E_Weapon(Weapon wep) {
  weapon = wep;
  switch ( weapon ) {
  case Weapon::sword:
    SetSprite("Images\\Big_Sword.png");
    cooldown = 2;
    SetSize(MathUtil::PixelsToWorldUnits(20),
            MathUtil::PixelsToWorldUnits(43));
  break;
  }
  SetRotation(theEnemy->GetBody()->GetLinearVelocity().x>0?std::_Pi/2:std::_Pi+std::_Pi/2);
  hit = 0;
  SetIsSensor(1);
  InitPhysics();
  GetBody()->SetGravityScale(0);
}

void Hero::E_Weapon::Update(float dt) {
  if ( !hit )
    if ( GetBoundingBox().Intersects(Game::thePlayer->GetBoundingBox()) ) {
      Game::thePlayer->Add_Curr_Health(-10);
      Particles::Add_Bleed(Vec2i(Game::thePlayer->GetPosition().X,Game::thePlayer->GetPosition().Y),
        std::atan2f(GetPosition().Y - Game::thePlayer->GetPosition().Y,
                    GetPosition().X - Game::thePlayer->GetPosition().X));
      hit = 1;
    }
  cooldown -= dt;
  if ( cooldown <= 0 && theEnemy != nullptr ) {
    theEnemy->weapon = nullptr;
    Destroy();
  }
}


Hero::Enemy::Enemy() {
  theEnemy = this;
  SetPosition(Vector2(0, 0));
  SetDrawShape(ADS_Square);
  SetSize(1,1);
  SetColor(.2,.2,.2);
  SetFixedRotation(true);
  InitPhysics();
  mood_tester = new TextActor();
  theWorld.Add(mood_tester);
  // avoid contact with player
  auto fixture = GetBody()->GetFixtureList()->GetFilterData();
  fixture.groupIndex = -8;
  GetBody()->GetFixtureList()->SetFilterData(fixture);
  // set user data
  GetBody()->SetUserData(this);

  intro = true;
  speed = 3;
  health = 20;

  weapon = nullptr;
  weapon_type = Weapon::sword;

  movement_cooldown = melee_cooldown = range_cooldown = in_air_end = in_air_start =
  movement_attack_flinch = ghost_cooldown = platform_cooldown = slide_timer = slide_direction
  = ghost_timer = on_platform_timer = jumping_to_platform = slide_cooldown = 0;
  gibber_timer = utility::R_Rand()/25 + 2;

  mood = Mood::Close;

  mood_switch_timer = 0;
  jump_timer = 0;

  const int word_count = 15;
  std::string rand_words[] = {
    "ass", "catcher", "asdf", "fasdf", "haksdjf", "kas", "sniff", "what", "okay", "bleh", "aklsdafsadfasdf", "deez_nutz", "what", "Your_Name_Here", "fuck",
    
  };

  // random name generation
  int num_words = utility::rand()%2+1; // 1-3 words
  for (int i = 0; i < num_words; i++) {
    int word_index = utility::rand()%word_count;
    int space = utility::rand()%2;
    if (space == 1 && i != 0) {
      name += "_";
    }

    name += rand_words[word_index];
  }

  int numbers = utility::rand()%4;
  if (numbers == 2) {
    name += std::to_string(utility::rand()%127);
  }

  std::cout << name << '\n';
};

void Hero::Enemy::Update(float dt) {
  if ( dt > .25 ) // throw this frame away
    dt = .25;
  int direction = 1;
  float distance_x = Game::thePlayer->GetPosition().X - GetPosition().X,
        distance_y = GetPosition().Y - Game::thePlayer->GetPosition().Y;

  if ( weapon ) {
    weapon->SetPosition(GetPosition());
  }

  gibber_timer -= dt;

  if ( gibber_timer <= 0 ) {
    gibber_timer = utility::R_Rand()/25 + 2;
    Sounds::Play_Gibberish();
  }

  melee_cooldown -= dt;
  range_cooldown -= dt;
  movement_cooldown -= dt;
  movement_attack_flinch -= dt;
  platform_cooldown -= dt;
  ghost_cooldown -= dt;
  in_air_start -= dt;
  in_air_end -= dt;
  slide_cooldown -= dt;

  mood_switch_timer -= dt;
  ApplyForce(Vector2(-GetBody()->GetLinearVelocity().x*dt*12,0),Vector2(0,0));

  mood_tester->SetPosition(Vector2(GetPosition().X,GetPosition().Y+3.5));


  if ( mood_switch_timer <= 0 ) {
    mood = (Mood)(int(utility::R_Rand())%(int(Mood::Size)));
    mood_switch_timer = 2.5;
    if ( !slide_direction > 0 && !jumping_to_platform > 0 )
      switch ( mood ) {
        case Mood::Close:
          SetColor(1,0,0);
        break;
        case Mood::Fair:
          SetColor(0,1,0);
        break;
        case Mood::Distant:
          SetColor(0,0,1);
        break;
      }
  }

  // performing abilities

  if ( ghost_timer > 0 ) {
    ghost_timer -= dt;
    // fade to alpha
    float alpha = (ghost_timer-3)/3;
    SetAlpha(alpha<.2?.2:alpha);
    if ( ghost_timer < 0 )
      SetAlpha(1);
  }

  if ( slide_timer > 0 ) {
    slide_timer -= dt;
    Apply_Vel_X(slide_direction?-.06:.06, dt);
    if ( slide_timer < 0 ) {
      switch ( mood ) {
        case Mood::Close:
          SetColor(1,0,0);
        break;
        case Mood::Fair:
          SetColor(0,1,0);
        break;
        case Mood::Distant:
          SetColor(0,0,1);
        break;
      }
    }
    return; // can't attack or move
  }

  if ( jumping_to_platform > 0 ) {
    if ( GetPosition().Y > 3 ) {
      GetBody()->SetLinearVelocity(b2Vec2(0,0));
      jumping_to_platform = 0;
      on_platform_timer = .001;
      platform = nullptr;
      // in case he was swiped out of the air we check
      if ( GetPosition().X < -7.5 && GetPosition().X > -9.5  ) {
        on_platform_timer = 5 + utility::R_Rand()/20;
        platform = new Level::Platform();
        platform->SetSize(1.5,.1);
        platform->SetPosition(-8,.7);
        platform->InitPhysics();
        platform->SetAlpha(0);
        platform->GetBody()->SetGravityScale(0);
        theWorld.Add(platform);
      }
      if ( GetPosition().X <  9.5 && GetPosition().X > 7.5 ) {
        on_platform_timer = 5 + utility::R_Rand()/20;
        platform = new Level::Platform();
        platform->SetSize(1.5,.1);
        platform->SetPosition( 8,.7);
        platform->InitPhysics();
        platform->GetBody()->SetGravityScale(0);
        platform->SetAlpha(0);
        theWorld.Add(platform);
      }
    } else {
      ApplyForce(Vec2i(0,20),Vec2i(0,0));
    }
    return; // can't attack or move
  }

  if ( on_platform_timer > 0 ) {
    on_platform_timer -= dt;
    if ( on_platform_timer <= 0 ) {
      if ( platform )
        platform->Destroy();
      switch ( mood ) {
        case Mood::Close:
          SetColor(1,0,0);
        break;
        case Mood::Fair:
          SetColor(0,1,0);
        break;
        case Mood::Distant:
          SetColor(0,0,1);
        break;
      }
      platform_cooldown = 6;
    }
    goto SKIP_MOVEMENT_PHASE;
  }

  // decide to move
  float dir = distance_x > 0 ? 1 : -1;
  switch ( mood ) {
    case Mood::Close: // up close and personal
      if ( abs(distance_x) < 1 ) { // maybe a bit too close
        Apply_Vel_X(.042*dir,dt);
        mood_tester->SetDisplayString("1");
        break;
      }
      if ( abs(distance_x) < 2 && abs(distance_x > 3) ) { // perfect
        mood_tester->SetDisplayString("2");
        break;
      }
      if ( abs(distance_x) < 4 ) { // I can inch up a bit closer
        Apply_Vel_X(.037*dir,dt);
        mood_tester->SetDisplayString("3");
        break;
      }
      // I'm too far away.
      Apply_Vel_X(.048*dir,dt);
      mood_tester->SetDisplayString("4");
    break;
    case Mood::Fair: // stab and throw
      if ( abs(distance_x) < 6 ) { // Back up a bit
        Apply_Vel_X(-0.037*dir,dt);
        mood_tester->SetDisplayString("1");
        break;
      }
      if ( abs(distance_x < 8 ) ) { // I can walk up
        Apply_Vel_X(0.038*dir,dt);
        mood_tester->SetDisplayString("2");
        break;
      }

      // I'm too far away
      mood_tester->SetDisplayString("3");
      Apply_Vel_X(0.048*dir,dt);
    break;
    case Mood::Distant:
      if ( abs(distance_x) < 3 ) { // I'm too close
        Apply_Vel_X(0.05*-dir,dt);
        mood_tester->SetDisplayString("1");
        break;
      }
      if ( abs(distance_x) < 8 ) { // back up
        Apply_Vel_X(0.042*-dir,dt);
        mood_tester->SetDisplayString("2");
        break;
      }
      if ( abs(distance_x) > 13 ) { // get close
        Apply_Vel_X(0.0395*dir,dt);
        mood_tester->SetDisplayString("3");
        break;
      }
    break;
  }

  SKIP_MOVEMENT_PHASE:;

  // ability options
  
  jump_timer -= dt;

  if ( jump_timer < 0 ) {
    jump_timer = .6 + utility::R_Rand()/(25 + (distance_x>3?0:25));
    theSound.PlaySound(Sounds::hero_jump, .1);
    in_air_start = .2;
    in_air_end   = .4;
    ApplyForce(Vec2i(0,1200),Vec2i(0,0));
  }

  // speed up 
  if ( (abs(distance_x) < 3 && (GetPosition().X > 8 || GetPosition().X < -8)) ) {
    slide_cooldown -= dt*2;
  }

  if ((jumping_to_platform <=  0 && slide_cooldown < 0) &&
      (utility::R_Rand() < 50 ||
      (abs(distance_x) < 3 && (GetPosition().X > 8 || GetPosition().X < -8))) ) {
    slide_cooldown = .8 + utility::R_Rand()/33;
    slide_timer = .6;
    theSound.PlaySound( Sounds::hero_slide, .1 );
    SetColor(.7,.7,.7);
    slide_direction = (distance_x < 0); // random
    if ( GetPosition().X >  8 ) // too close to right wall
      slide_direction = 1;
    else if ( GetPosition().X < -8 ) // too close to left wall
      slide_direction = 0;
    else
      SetColor(0,0,0);
  }

  if ( ghost_cooldown < 0 ) {
    // ghost
    if ( utility::R_Rand() < 5 && ghost_timer <= 0 ) {
      ghost_cooldown = 2;
      ghost_timer = 5 + utility::R_Rand()/10;
    }
  }
  if ( platform_cooldown < 0 ) {
    // jump to platform and player is under one of the platforms
    if ( on_platform_timer <= 0 &&
          (( GetPosition().X > -9 && GetPosition().X < -7 ) ||
           ( GetPosition().X <  9 && GetPosition().X >  7 )) ) {
      theSound.PlaySound(Sounds::hero_jump, .1);
      jumping_to_platform = 1;
      platform_cooldown = 5;
      on_platform_timer = 13;
      GetBody()->SetLinearVelocity(b2Vec2(0,0));
      SetColor(1,1,0);
      slide_timer = -1;
      return;
    }
  }

  // attacks options
  if ( movement_attack_flinch > 0 ) return; // hero just attacked
  if ( melee_cooldown < 0 && abs(distance_x) <= 4 &&
        in_air_start < 0 && in_air_end > 0) {
    Attack_Melee();
    theSound.PlaySound( Sounds::hero_attack, .1 );
    return;
  }
  if ( range_cooldown < 0 && abs(distance_x) > 2.3 ) {
    theSound.PlaySound( Sounds::hero_throw, .1 );
    Attack_Range();
    return;
  }

  if ( health <= 0 ) {
    Killed();
  }
}

void Hero::Enemy::Apply_Vel_X(float x, float dt) {
  x *= .96;
  ApplyLinearImpulse(Vector2(x*dt*500,0),Vector2(0,0));
}

void Hero::Enemy::Jump() {
  
}

void Hero::Enemy::Attack_Melee() {
  melee_cooldown = 5;
  auto z = new E_Weapon(weapon_type);//new Dagger(std::atan2f(Game::thePlayer->GetPosition().Y-GetPosition().Y,
           //                       Game::thePlayer->GetPosition().X-GetPosition().X),
           //           Vec2i(GetPosition().X,GetPosition().Y));
  theWorld.Add(z);
}

void Hero::Enemy::Attack_Range() {
  range_cooldown = 7;
  auto z = new Dagger(std::atan2f(Game::thePlayer->GetPosition().Y-GetPosition().Y,
                                  Game::thePlayer->GetPosition().X-GetPosition().X),
                      Vec2i(GetPosition().X,GetPosition().Y));
  theWorld.Add(z);
}

Hero::Enemy::~Enemy() {
  if ( platform )
    this->platform->Destroy();
  mood_tester->Destroy();
}

void Hero::Enemy::Add_Health(int x) {
  if ( x < 0 && (health + x > 0) )
    theSound.PlaySound( Sounds::hero_damaged, .1 );
 health += x;
 if ( health <= 0 )
   theSound.PlaySound( Sounds::hero_death );
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
  //SetSize(MathUtil::ScreenToWorld(Vec2i(100,0)).X,
  //        MathUtil::ScreenToWorld(Vec2i(0, 20)).Y);
  SetDrawShape(ADS_Square);
  SetDensity(0.01f);
  SetIsSensor(1);
  InitPhysics();
  //this->GetBody()->SetTransform(GetBody()->GetPosition(), angl );
  ApplyForce(Vec2i(std::cos(angl)*5,
                   std::sin(angl)*5),
                   Vector2(0,0));
  lifetime = 25;
  // set user data
  GetBody()->SetUserData(this);
  hit = 0;
}

void Hero::Dagger::Update(float dt) {
  lifetime -= dt;
  if ( lifetime < 0 || Hero::theEnemy == nullptr ) {
    Destroy();
  }

  if ( !hit )
    if ( GetBoundingBox().Intersects(Game::thePlayer->GetBoundingBox()) ) {
      Game::thePlayer->Add_Curr_Health(-10);
      Particles::Add_Bleed(Vec2i(theEnemy->GetPosition().X,theEnemy->GetPosition().Y),
        std::atan2f(Game::thePlayer->GetPosition().Y - GetPosition().Y,
                    Game::thePlayer->GetPosition().X - GetPosition().X));
      hit = 1;
    }
}