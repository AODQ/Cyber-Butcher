#include "stdafx.h"
#include "Angel.h"
#include "Augments.h"
#include "LD33.h"

Augments::Head::Head(Augments::Head_Type x) : type(x) {
  switch ( x ) {
    case Head_Type::head:
      SetSprite("Images\\head.png");
    break;
  };
};
  
void Augments::Head::Update(float dt) {
  SetPosition(Game::thePlayer->GetPosition().X,
              Game::thePlayer->GetPosition().Y-
                MathUtil::ScreenToWorld(Vec2i(0,40)).Y);
};
  
Augments::Body::Body(Augments::Body_Type x) : type(x) {
  switch ( x ) {
    case Body_Type::body:
      SetSprite("Images\\body.png");
    break;
  };
}
  
void Augments::Body::Update(float dt) {
  SetPosition(Game::thePlayer->GetPosition().X,
              Game::thePlayer->GetPosition().Y);
}

Augments::Weapon::Weapon(Augments::Weapon_Type x ) : type(x) {
  switch ( x ) {
    case Weapon_Type::sword:
      SetSprite("Images\\weapon.png");
    break;
  }
}
  
void Augments::Weapon::Update(float dt) {
  SetPosition(Game::thePlayer->GetPosition().X,
              Game::thePlayer->GetPosition().Y-
                MathUtil::ScreenToWorld(Vec2i(0,-40)).Y);
}
