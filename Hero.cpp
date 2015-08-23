#include "stdafx.h"
#include "Hero.h"
#include "Angel.h"
#include "utility.h"
#include "Monster.h"
#include "LD33.h"

Hero::Enemy::Enemy() {
  theEnemy = this;
  SetPosition(Vector2(-11.875, -1.5625));
  SetDrawShape(ADS_Square);
  SetSize(1,1);
  SetColor(.2,.2,.2);
  InitPhysics();
  // avoid contact from player
  auto fixture = GetBody()->GetFixtureList()->GetFilterData();
  fixture.groupIndex = -8;
  GetBody()->GetFixtureList()->SetFilterData(fixture);
  // set user data
  GetBody()->SetUserData(this);

  health = 1;
};

/*Hero::Enemy_Intro::Enemy_Intro() {
  SetDrawShape(ADS_Square);
  SetSize(1,1);
  SetColor(.2,.2,.2);
  // set position to left of screenby 100 and move into screen area
  SetPosition(Vector2(-15, -1.5625));
  MoveTo(Vector2(-11.875, -1.5625),
          2.5f,1,"FinishedHeroMovement");
  std::cout << GetPosition().X;
}*/

void Hero::Enemy::Update(float dt) {

  //if (!intro) {
    // just walk towards the enemy
    ApplyForce(Vec2i(10*(Game::thePlayer->GetPosition().X - GetPosition().X),
                     0),Vector2(0,0));

    // abilities
    if ( ab1_cooldown >= 0 ) ab1_cooldown -= dt;
    // basic attack (don't even worry about ab1/2 shit)
    if ( int(utility::rand()) < 2 && ab1_cooldown <= 0 ) {
      Cast_Ability(Ability::dagger_throw);
      ab1_cooldown = 300;
    }
    if ( ab2_cooldown >= 0 ) ab2_cooldown -= dt;
    if ( int(utility::rand()) < 5 && ab2_cooldown <= 0 ) {
      ApplyForce(Vec2i(30*(Game::thePlayer->GetPosition().X - GetPosition().X),
                       0),Vector2(0,0));
      ab2_cooldown = 1500;
    }

    if ( health <= 0 ) {
      Killed();
    }
  //}
};


void Hero::Cast_Ability(Ability ab) {
  switch ( ab ) {
    case Ability::dagger_throw:
      float dx = Game::thePlayer->GetPosition().X-theEnemy->GetPosition().X,
            dy = Game::thePlayer->GetPosition().Y-theEnemy->GetPosition().Y;
      
      float ang = std::atan2f(dy,dx);
      theWorld.Add(new Dagger(ang,Vec2i(theEnemy->GetPosition().X+std::cos(ang),
                                        theEnemy->GetPosition().Y+std::sin(ang))
                              ));
    break;
  }
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
    theEnemyIntro->Destroy();
    theEnemyIntro = nullptr;
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
Hero::Enemy_Intro* Hero::theEnemyIntro = nullptr;


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