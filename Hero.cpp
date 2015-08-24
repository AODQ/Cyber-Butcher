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
  SetSize(MathUtil::PixelsToWorldUnits(20),
          MathUtil::PixelsToWorldUnits(20));
  switch ( weapon ) {
  case Weapon::dagger:
    SetSprite("Images\\Hero_Weapon1.png");
    cooldown = 6;
  break;
  case Weapon::sword:
    SetSprite("Images\\Hero_Weapon2.png");
    cooldown = 6;
  break;
  case Weapon::broadsword:
    SetSprite("Images\\Hero_Weapon3.png");
    cooldown = 6;
  break;
  case Weapon::miniaxe:
    SetSprite("Images\\Hero_Weapon4.png");
    cooldown = 6;
  break;
  case Weapon::axe:
    SetSprite("Images\\Hero_Weapon5.png");
    cooldown = 6;
  break;
  case Weapon::bigaxe:
    SetSprite("Images\\Hero_Weapon6.png");
    cooldown = 6;
  break;
  case Weapon::spear:
    SetSprite("Images\\Hero_Weapon7.png");
    cooldown = 6;
  break;
  }

  hit = 0;
  SetPosition(Hero::theEnemy->GetPosition().X-
        (Hero::theEnemy->GetBody()->GetLinearVelocity().x > 0 ? 1 : -1 *
            MathUtil::PixelsToWorldUnits(5)),
         GetPosition().Y-MathUtil::PixelsToWorldUnits(25)),
  SetIsSensor(1);
  InitPhysics();
  GetBody()->SetGravityScale(0);
}

void Hero::E_Weapon::Update(float dt) {
  if ( !hit && int(Hero::theEnemy->attack_animation) >= 4 )
    if ( GetBoundingBox().Intersects(Game::thePlayer->GetBoundingBox()) ) {
      Game::thePlayer->Add_Curr_Health(-10);
      Particles::Add_Bleed(Vec2i(Game::thePlayer->GetPosition().X,Game::thePlayer->GetPosition().Y),
        std::atan2f(GetPosition().Y - Game::thePlayer->GetPosition().Y,
                    GetPosition().X - Game::thePlayer->GetPosition().X));
      hit = 1;
    }
  if ( theEnemy != nullptr ) {
    if ( Hero::theEnemy->attack_animation >= 6 ) {
      theEnemy->weapon = nullptr;
      Destroy();
    }
  } else
    Destroy();
}


Hero::E_Throw::E_Throw(Hero::Throwable th, float ang) {
  SetSize(MathUtil::PixelsToWorldUnits(32),
          MathUtil::PixelsToWorldUnits(32));
  SetIsSensor(1);
  SetPosition(theEnemy->GetPosition());
  SetDensity(0.2);
  InitPhysics();
  type = th;
  cooldown = 5;
  hit = 0;
  float angle = std::atan2f(Game::thePlayer->GetPosition().Y-theEnemy->GetPosition().Y,
                          Game::thePlayer->GetPosition().X-theEnemy->GetPosition().X);
  switch ( th ) {
    case Throwable::axe:
      if ( angle < std::_Pi/2 ) angle += std::_Pi/5;
      else                      angle -= std::_Pi/5;
      SetSprite("Images\\Hero_Throwing_Axe.png");
      ApplyForce(Vector2(std::cos(angle)*350,std::sin(angle)*850),Vector2(0,0));
      ApplyTorque(50);
    break;
    case Throwable::knife:
      SetSprite("Images\\Hero_Throwing_Knife.png");
      GetBody()->SetTransform(GetBody()->GetPosition(),angle);
      ApplyForce(Vector2(std::cos(angle)*350,std::sin(angle)*350),Vector2(0,0));
      GetBody()->SetGravityScale(0);
      cooldown = 1;
    break;
    case Throwable::ball:
      angle -= ang;
      SetSprite("Images\\Hero_Throwing_Magic_00" + std::to_string(int(utility::R_Rand())%3+1) + ".png");
      ApplyForce(Vector2(std::cos(angle)*200,std::sin(angle)*200),Vector2(0,0));
      GetBody()->SetGravityScale(0);
      cooldown = 2;
    break;
    case Throwable::cross:
      SetSprite("Images\\Hero_Throwing_Shuriken.png");
      origin = GetPosition();
      ApplyTorque(50);
      recoiled = 0;
      GetBody()->SetGravityScale(0);
      ApplyForce(Vector2(std::cos(angle)*500,std::sin(angle)*500),Vector2(0,0));
    break;
  }
}

void Hero::E_Throw::Update(float dt) {
  switch ( type ) {
    case Throwable::cross:
      ApplyForce(Vector2(-GetBody()->GetLinearVelocity().x*dt*50,
                         -GetBody()->GetLinearVelocity().y*dt*50),Vector2(0,0));
      if ( abs(GetBody()->GetLinearVelocity().x) +
           abs(GetBody()->GetLinearVelocity().y) < 0.5 ) {
        if ( !recoiled ) {
          GetBody()->SetLinearVelocity(b2Vec2(0,0));
          float angle = std::atan2f(GetPosition().Y - origin.Y,
                                    GetPosition().X - origin.X);
          ApplyForce(Vector2(std::cos(angle)*100,std::sin(angle)*100),Vector2(0,0));
          GetBody()->SetAngularVelocity(0);
          ApplyTorque(-50);
        } else
          this->Destroy();
      }
    break;
    default:
      cooldown -= dt;
      if ( cooldown <= 0 )
        this->Destroy();
  }
  if ( !hit )
    if ( GetBoundingBox().Intersects(Game::thePlayer->GetBoundingBox()) ) {
      hit = 1;
      Game::thePlayer->Add_Curr_Health(-3);
    }
}

Hero::Enemy::Enemy() {
  theEnemy = this;
  SetPosition(Vector2(0, 0));
  SetFriction(0);
  SetSize(1,1);
  draw_me = new Actor();
  draw_me->SetSize(MathUtil::PixelsToWorldUnits(32),
                   MathUtil::PixelsToWorldUnits(32));
  SetAlpha(0);
  theWorld.Add(draw_me);
  SetFixedRotation(true);
  InitPhysics();
  // avoid contact with player
  auto fixture = GetBody()->GetFixtureList()->GetFilterData();
  fixture.groupIndex = -8;
  GetBody()->GetFixtureList()->SetFilterData(fixture);

  intro = true;
  speed = 3;
  health = 20;

  weapon = nullptr;
  weapon_type = Weapon   (int(utility::R_Rand())%int(Weapon::size));
  throw_type  = Throwable(int(utility::R_Rand())%int(Throwable::size));

  movement_cooldown = melee_cooldown = range_cooldown = in_air_end = in_air_start =
  movement_attack_flinch = ghost_cooldown = platform_cooldown = slide_timer = slide_direction
  = ghost_timer = on_platform_timer = jumping_to_platform = slide_cooldown = 0;
  gibber_timer = utility::R_Rand()/10 + 5;
  
  ghost_timer = 15 + utility::R_Rand()/20;
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
  platform = nullptr;
  std::cout << name << '\n';
};

void Hero::Enemy::Update(float dt) {
  if ( Game::thePlayer == nullptr ) {
    Destroy();
    return;
  }
  if ( dt > .25 ) // throw this frame away
    dt = .25;
  int direction = 1;
  float distance_x = Game::thePlayer->GetPosition().X - GetPosition().X,
        distance_y = GetPosition().Y - Game::thePlayer->GetPosition().Y;
  if ( on_platform_timer <= 0 ) {
    if ( platform ) {
      platform->Destroy();
      platform = nullptr;
    }
  }
  gibber_timer -= dt;

  if ( gibber_timer <= 0 ) {
    gibber_timer = utility::R_Rand()/10 + 5;
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

  if ( mood_switch_timer <= 0 ) {
    mood = (Mood)(int(utility::R_Rand())%(int(Mood::Size)));
    mood_switch_timer = 2.5;
  }

  // performing abilities

  if ( ghost_timer > 0 ) {
    ghost_timer -= dt;
    // fade to alpha
    float alpha = (ghost_timer-3)/3;
    draw_me->SetAlpha(alpha<.2?.2:alpha);
    if ( ghost_timer < 0 )
      draw_me->SetAlpha(1);
  }

  if ( slide_timer > 0 ) {
    slide_timer -= dt;
    Apply_Vel_X(slide_direction?-.06:.06, dt);
    goto DETERMINE_SPRITE; // can't attack or move
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
    goto DETERMINE_SPRITE; // can't attack or move
  }

  if ( on_platform_timer > 0 ) {
    on_platform_timer -= dt;
    if ( on_platform_timer <= 0 ) {
      platform_cooldown = 6;
    }
    goto SKIP_MOVEMENT_PHASE;
  }

  // decide to move
  float dir = distance_x > 0 ? 1 : -1;
  switch ( mood ) {
    case Mood::Close: // up close and personal
      if ( abs(distance_x) < 1 ) { // maybe a bit too close
        Apply_Vel_X(.052*dir,dt);
        break;
      }
      if ( abs(distance_x) < 2 && abs(distance_x > 3) ) { // perfect
        break;
      }
      if ( abs(distance_x) < 4 ) { // I can inch up a bit closer
        Apply_Vel_X(.057*dir,dt);
        break;
      }
      // I'm too far away.
      Apply_Vel_X(.058*dir,dt);
    break;
    case Mood::Fair: // stab and throw
      if ( abs(distance_x) < 6 ) { // Back up a bit
        Apply_Vel_X(-0.057*dir,dt);
        break;
      }
      if ( abs(distance_x < 8 ) ) { // I can walk up
        Apply_Vel_X(0.058*dir,dt);
        break;
      }

      // I'm too far away
      Apply_Vel_X(0.048*dir,dt);
    break;
    case Mood::Distant:
      if ( abs(distance_x) < 3 ) { // I'm too close
        Apply_Vel_X(0.07*-dir,dt);
        break;
      }
      if ( abs(distance_x) < 8 ) { // back up
        Apply_Vel_X(0.062*-dir,dt);
        break;
      }
      if ( abs(distance_x) > 13 ) { // get close
        Apply_Vel_X(0.0595*dir,dt);
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
    in_air_end   = jump_timer - .2;
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
    slide_direction = (distance_x < 0); // random
    if ( GetPosition().X >  8 ) // too close to right wall
      slide_direction = 1;
    else if ( GetPosition().X < -8 ) // too close to left wall
      slide_direction = 0;
  }

  if ( ghost_cooldown < 0 ) {
    // ghost
    if ( utility::R_Rand() < 5 && ghost_timer <= 0 ) {
      ghost_cooldown = 7 + utility::R_Rand()/10;
      ghost_timer = 15 + utility::R_Rand()/20;
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
      slide_timer = -1;
      goto DETERMINE_SPRITE;
    }
  }

  // attacks options
  if ( movement_attack_flinch > 0 ) return; // hero just attacked
  if ( melee_cooldown < 0 && abs(distance_x) <= 4 &&
        in_air_start < 0 && in_air_end > 0) {
    Attack_Melee();
    theSound.PlaySound( Sounds::hero_attack, .1 );
    goto DETERMINE_SPRITE;
  }
  if ( range_cooldown < 0 && abs(distance_x) > 2.3 ) {
    theSound.PlaySound( Sounds::hero_throw, .1 );
    Attack_Range();
    goto DETERMINE_SPRITE;
  }

  if ( health <= 0 ) {
    Killed();
  }

  DETERMINE_SPRITE:
  // idle ?
  auto prev_state = state;
  auto prev_walk  = walk_animation;
  auto prev_attack= attack_animation;
  walk_animation += dt*10;
  if ( abs(GetBody()->GetLinearVelocity().x) < .0001 )
    state = Anim_State::idle;
  else {  // walking
    state = Anim_State::walk;
    if ( walk_animation >= 8 )
      walk_animation = 0;
  }
  // stuff below takes precedence over the above in order of most importance to least
  if ( in_air_end > 0 )
    state = Anim_State::jump;
  if ( slide_timer > 0 )
    state = Anim_State::slide;
  if ( attack_animation > 0 ) {
    state = Anim_State::attack;
    attack_animation += 10 * dt;
  }
    

  
  switch ( state ) {
    case Anim_State::idle:
      if ( prev_state != state ) draw_me->SetSprite("Images\\Hero_Idle.png"); break;
    case Anim_State::walk:
      if ( int(walk_animation) != int(prev_walk) ) {
        draw_me->SetSprite("Images\\Hero_Walk_00" + std::to_string(int(walk_animation)+1) + ".png");
      }
    break;
    case Anim_State::jump:
      if ( prev_state != state )
        draw_me->SetSprite("Images\\Hero_Jump.png"); break;
    case Anim_State::slide:
      if ( prev_state != state )
        draw_me->SetSprite("Images\\Hero_Slide.png"); break;
    case Anim_State::attack:
      if ( attack_animation > 7 ) {
        attack_animation = 0;
        break;
      } 
      if ( int(attack_animation) != int(prev_attack) )
        draw_me->SetSprite("Images\\Hero_Attack_00" + 
                  std::to_string(int(attack_animation>=3?3:attack_animation)+1) + ".png");
      if ( weapon != nullptr ) { // melee weapon was used
        float direction = GetBody()->GetLinearVelocity().x > 0 ? 1 : -1;
        switch ( int(attack_animation) ) {
          case 0:
            weapon->GetBody()->SetTransform(
              b2Vec2(GetPosition().X+(direction*MathUtil::PixelsToWorldUnits(8)),
                     GetPosition().Y+MathUtil::PixelsToWorldUnits(7)),
             0/*std::_Pi/2*/);
          break;
          case 1:
            weapon->GetBody()->SetTransform(
              b2Vec2(GetPosition().X+(direction*MathUtil::PixelsToWorldUnits(-8)),
                     GetPosition().Y+MathUtil::PixelsToWorldUnits(22)),
              direction*std::_Pi/4);
          break;
          case 2:
            weapon->GetBody()->SetTransform(
              b2Vec2(GetPosition().X+(direction*MathUtil::PixelsToWorldUnits(5)),
                     GetPosition().Y+MathUtil::PixelsToWorldUnits(16)),
              -direction*std::_Pi/10);
          break;
          default:
            weapon->GetBody()->SetTransform(
              b2Vec2(GetPosition().X+(direction*MathUtil::PixelsToWorldUnits(15)),
                     GetPosition().Y+MathUtil::PixelsToWorldUnits(15)),
              -direction*std::_Pi/2);
            // check for collision
            if ( !wep_hit ) {
              auto tz = new PhysicsActor();
              float sx = 0,
                    sy = 0;
              switch ( weapon_type ) {
                case Weapon::axe:
                  sx = 3;
                  sy = 6;
                break;
                case Weapon::bigaxe:
                  sx = 6;
                  sy = 6;
                break;
                case Weapon::miniaxe:
                  sx = 2;
                  sy = 4;
                break;
                case Weapon::broadsword:
                  sx = 13;
                  sy = 4;
                break;
                case Weapon::sword:
                  sx = 4;
                  sy = 4;
                break;
                case Weapon::dagger:
                  sy = 3;
                  sx = 3;
                break;
                case Weapon::spear:
                  sy = 3;
                  sx = 13;
                break;
              }
              tz->SetSize(MathUtil::PixelsToWorldUnits(sx),
                          MathUtil::PixelsToWorldUnits(sy));
              tz->SetPosition(GetPosition().X + direction*MathUtil::PixelsToWorldUnits(3),
                              GetPosition().Y);
              tz->SetIsSensor(1);
              tz->InitPhysics();
              if ( tz->GetBoundingBox().Intersects(Game::thePlayer->GetBoundingBox()) ) {
                Game::thePlayer->Add_Curr_Health(-3);
                wep_hit = 1;
              }
              tz->Destroy();
            }
          break;
        }
    }
    break;
  }
  draw_me->SetPosition(GetPosition().X,GetPosition().Y+MathUtil::PixelsToWorldUnits(8));
  if ( state == Anim_State::slide )
    draw_me->SetPosition(GetPosition().X,GetPosition().Y+MathUtil::PixelsToWorldUnits(12));
  if ( GetBody()->GetLinearVelocity().x > 0 ) {
    draw_me->SetUVs(Vector2(0.f, 0.f), Vector2(1.f, 1.f));
  } else
    draw_me->SetUVs(Vector2(1.f, 0.f), Vector2(0.f, 1.f));
}

void Hero::Enemy::Apply_Vel_X(float x, float dt) {
  x *= .96;
  ApplyLinearImpulse(Vector2(x*dt*500,0),Vector2(0,0));
}

void Hero::Enemy::Jump() {
  
}

void Hero::Enemy::Attack_Melee() {
  melee_cooldown = 5;
  wep_hit = 0;
  weapon = new E_Weapon(weapon_type);
  theWorld.Add(weapon);
  attack_animation = .00001;
}

void Hero::Enemy::Attack_Range() {
  range_cooldown = 7;
  attack_animation = .0001;
  auto z = new E_Throw(throw_type);
  if ( throw_type == Throwable::ball ) {
    z = new E_Throw(throw_type,std::_Pi/2);
    theWorld.Add(z);
    z = new E_Throw(throw_type,std::_Pi/-2);
    theWorld.Add(z);
  }

}

Hero::Enemy::~Enemy() {
  if ( platform )
    this->platform->Destroy();
  draw_me->Destroy();
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
  Particles::Add_Bleed(Vec2i(GetPosition().X,GetPosition().Y),std::_Pi/2,4000);
  theWorld.Add(new Enemy_Death());

  /*for ( int i = 0; i != 15; ++ i ) {
    Hero::Gold* dropped_gold = new Hero::Gold(this->GetPosition());
    theWorld.Add(dropped_gold);
    dropped_gold->ApplyForce(Vector2((std::rand()-50)/20,std::rand()/25),Vector2(0,0));
  }*/

  this->Destroy();
  Game::theOverseer->level++;
  theEnemy = nullptr;
  Game::theKeep->time_left = 10.0f;
  //Game::theKeep->NewItems();
}

Hero::Enemy_Death::Enemy_Death() {
  SetSize(MathUtil::PixelsToWorldUnits(32),
          MathUtil::PixelsToWorldUnits(32));
  SetSprite("Images\\Hero_Death.png");
  SetPosition(theEnemy->GetPosition());
  auto fixture = GetBody()->GetFixtureList()->GetFilterData();
  fixture.groupIndex = -8;
  GetBody()->GetFixtureList()->SetFilterData(fixture);
  InitPhysics();
  time = 5;
}
void Hero::Enemy_Death::Update(float x) {
  time -= x;
  if ( time <= 0 )
    Destroy();
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