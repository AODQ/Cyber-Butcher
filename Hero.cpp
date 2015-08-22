#include "stdafx.h"
#include "Hero.h"
#include "Angel.h"
#include "Utility.h"
#include "Monster.h"
#include "LD33.h"

Hero::Enemy::Enemy() {
  theEnemy = this;
  SetPosition(MathUtil::ScreenToWorld(50,utility::Window_height-20));
  SetDrawShape(ADS_Square);
  SetSize(1,1);
  SetColor(.2,.2,.2);
  InitPhysics();
  auto fixture = theEnemy->GetBody()->GetFixtureList()->GetFilterData();
  fixture.groupIndex = -8;
  theEnemy->GetBody()->GetFixtureList()->SetFilterData(fixture);
};

Hero::Enemy_Intro::Enemy_Intro() {
  SetDrawShape(ADS_Square);
  SetSize(1,1);
  SetColor(.2,.2,.2);
  // set position to left of screenby 100 and move into screen area
  SetPosition(MathUtil::ScreenToWorld(-100,utility::Window_height-20));
  MoveTo(MathUtil::ScreenToWorld(50,utility::Window_height-20),
          2.5f,1,"FinishedHeroMovement");
}

void Hero::Enemy::Update(float dt) {
  // just walk towards the enemy
  ApplyForce(Vec2i(.2*(Game::thePlayer->GetPosition().X - GetPosition().X),
                   0),Vector2(0,0));
  // basic attack (don't even worry about ab1/2 shit)
  if ( int(utility::rand()) == 0 ) {
    
  }
  if ( int(utility::rand()) == 1 ) {
    ApplyForce(Vec2i(30*(Game::thePlayer->GetPosition().X - GetPosition().X),
                     0),Vector2(0,0));
  }
};


void Hero::Cast_Ability(Ability ab, Enemy* en) {
  switch ( ab ) {
    case Ability::dagger_throw:
      
    break;
  }
}

void Hero::Enemy_Listener::ReceiveMessage(Message* m) {
  if ( m->GetMessageName() == std::string("FinishedHeroMovement") ) {
    theEnemy = new Enemy();
    theWorld.Add(theEnemy);
    theEnemyIntro->Destroy();
    theEnemyIntro = nullptr;
  }
}


Hero::Enemy* Hero::theEnemy = nullptr;
Hero::Enemy_Listener* Hero::e_listener = nullptr;
Hero::Enemy_Intro* Hero::theEnemyIntro = nullptr;


// enemy attacks

Hero::Dagger::Dagger(float angl, Vec2i pos) {
  SetPosition(pos);
  SetRotation(angl);
  InitPhysics();
  ApplyForce(Vec2i(std::cos(angl)*theTuning.GetFloat("DaggerForce"),
                   std::sin(angl)*theTuning.GetFloat("DaggerForce")),
                   Vector2(0,0));
  lifetime = theTuning.GetInt("DaggerLifetime");
}

void Hero::Dagger::Update(float dt) {
  if ( lifetime - dt < 0 ) {
    Destroy();
  }
  
}