#include "stdafx.h"
#include "Hero.h"
#include "Angel.h"
#include "utility.h"
#include "Monster.h"
#include "LD33.h"

Hero::Enemy::Enemy() {
  theEnemy = this;
  SetPosition(MathUtil::ScreenToWorld(0,185));
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
};

Hero::Enemy_Intro::Enemy_Intro() {
  SetDrawShape(ADS_Square);
  SetSize(1,1);
  SetColor(.2,.2,.2);
  // set position to left of screenby 100 and move into screen area
  SetPosition(MathUtil::ScreenToWorld(0,185));
  MoveTo(MathUtil::ScreenToWorld(50,185),
          2.5f,1,"FinishedHeroMovement");
}

void Hero::Enemy::Update(float dt) {
  // just walk towards the enemy
  ApplyForce(Vec2i(.02*(Game::thePlayer->GetPosition().X - GetPosition().X),
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