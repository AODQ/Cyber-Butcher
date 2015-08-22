#include "Augments.h"
#include "LD33.h"

namespace Augments {

  Head::Head(Head_Type::HEAD_TYPE x) : type(x) {
    switch ( x ) {
      case Head_Type::head:
        SetSprite("Images\\head.png");
      break;
    };
  };
  
  void Head::Update(float dt) {
    SetPosition(Game::thePlayer->GetPosition().X,
                Game::thePlayer->GetPosition().Y-MathUtil::ScreenToWorld(40));
  };
  
  Body::Body(Body_Type::BODY_TYPE x) : type(x) {
    switch ( x ) {
      case Body_Type::body:
        SetSprite("Images\\body.png");
      break;
    };
  }
  
  void Body::Update(float dt) {
    SetPosition(Game::thePlayer->GetPosition().X,
                Game::thePlayer->GetPosition().Y);
  }

  Weapon::Weapon(Weapon_Type::WEAPON_TYPE x ) : type(x) {
    switch ( x ) {
      case Weapon_Type::weapon:
        SetSprite("Images\\weapon.png");
      break;
    }
  }
  
  void Weapon::Update(float dt) {
    SetPosition(Game::thePlayer->GetPosition().X,
                Game::thePlayer->GetPosition().Y+MathUtil::ScreenToWorld(40));
  }
};