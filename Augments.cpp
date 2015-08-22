#include "stdafx.h"
#include "Angel.h"
#include "Augments.h"
#include "LD33.h"
#include "Hero.h"

// helper functions


float R_Off_X(Augments::Weapon_Type wt) {
  using namespace Augments;
  switch ( wt ) {
    case Weapon_Type::Big_Sword:
      return theTuning.GetFloat("BigSwordOffsetX");
  }
}


float R_Off_Y(Augments::Weapon_Type wt) {
  using namespace Augments;
  switch ( wt ) {
    case Weapon_Type::Big_Sword:
      return theTuning.GetFloat("BigSwordOffsetY");
  }
}




Augments::Weapon::Weapon(Augments::Weapon_Type x ) : type(x) {
  animation_timer = 0;
  coll_check = 0;
  switch ( x ) {
    case Weapon_Type::Big_Sword:
      this->SetSize(MathUtil::PixelsToWorldUnits(20),
                    MathUtil::PixelsToWorldUnits(43));
      SetSprite("Images\\Big_Sword.png");
    break;
  }
}
  
void Augments::Weapon::Update(float dt) {
  SetPosition(Game::thePlayer->GetPosition().X -
    (animation_timer != -100?origin_direction?1:-1:
            Game::thePlayer->R_Direction()?   1:-1)*R_Off_X(type),
              Game::thePlayer->GetPosition().Y - R_Off_Y(type));
  if ( animation_timer-dt > 0 ) {
    animation_timer -= dt;
    switch ( type ) {
      case Weapon_Type::Big_Sword:
        SetRotation(origin_direction?90 - // left
          abs(animation_timer-theTuning.GetFloat("BigSwordAnimationLength"))*90 :
          90+abs(animation_timer-theTuning.GetFloat("BigSwordAnimationLength"))*90);
      break;
    }
  } else if ( animation_timer != -100 ){
    animation_timer = -100;
    SetRotation(0);
  }

  if ( coll_check ) {
    auto z = coll_check;
    coll_check = nullptr;
    if ( z->GetBoundingBox().Intersects(Hero::theEnemy->GetBoundingBox()) ) {
          Hero::theEnemy->ApplyForce(Vector2((origin_direction?-1:1)*
            theTuning.GetFloat("BigSwordForce"), 2), Vector2(0,0));
          Hero::theEnemy->Add_Health(-theTuning.GetFloat("BigSwordDamage"));
    }
    z->Destroy();
  }
}

void Augments::Weapon::Cast() {
  switch ( type ) {
    case Weapon_Type::Big_Sword:
      animation_timer = theTuning.GetFloat("BigSwordAnimationLength");
      // check for collision
      if ( Hero::theEnemy != nullptr ) {
        coll_check = new PhysicsActor();
        auto z = coll_check;
        theWorld.Add(z);
        z->SetIsSensor(1);
        z->SetSize(MathUtil::PixelsToWorldUnits(20),
                   MathUtil::PixelsToWorldUnits(13));
        z->SetRotation(origin_direction ? 0 /*right*/ : 180);
        z->SetPosition(this->GetPosition());
        z->InitPhysics();
        origin_direction = Game::thePlayer->R_Direction();
      }
    break;
  }
}