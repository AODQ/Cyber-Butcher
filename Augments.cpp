#include "stdafx.h"
#include "Angel.h"
#include "Augments.h"
#include "LD33.h"
#include "Particle_System.h"
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

  if ( coll_check && Hero::theEnemy != nullptr ) {
    auto z = coll_check;
    coll_check = nullptr;
    if ( z->GetBoundingBox().Intersects(Hero::theEnemy->GetBoundingBox()) ) {
      Hero::theEnemy->ApplyForce(Vector2((origin_direction?-1:1)*
        theTuning.GetFloat("BigSwordForce"), 2), Vector2(0,0));
      Hero::theEnemy->Add_Health(-theTuning.GetFloat("BigSwordDamage"));
      Particles::Add_Bleed(Vec2i(Hero::theEnemy->GetPosition().X,
                                 Hero::theEnemy->GetPosition().Y),origin_direction);
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


// shop keep
Augments::ShopKeep::ShopKeep() {
  _active = false;
  time_left = 0;

  SetSprite("Images\\shopkeep.png");
  SetAlpha(0);
  SetSize(10, 10);
  SetPosition(0, 20);
}

void Augments::ShopKeep::Update(float dt) {
  if (time_left > 0) {
    time_left -= dt;
  }

  if (time_left <= 0 && _active) {
    // close shop
    SetPosition(0, 20);
    _active = false;
    for ( int i = 0; i != 3; ++ i )
      if ( items[i] != nullptr )
        items[i]->Destroy();
    items[0] = items[1] = items[2] = nullptr;
  } else if (time_left > 0 && !_active) {
    // open shop
    SetPosition(0, 0);
    _active = true;
  }
}

void Augments::ShopKeep::NewItems() {
  for ( int i = 0; i != 3; ++ i ) {
    auto z = new Shop_Item(Shop_Item_Type(int(utility::R_Rand())%int(Shop_Item_Type::Size)));
    z->SetPosition(-8 + i*7, -1);
    z->InitPhysics();
    z->GetBody()->SetGravityScale(0.0f);
    items[i] = z;
    theWorld.Add(z);
  }
}

Augments::Shop_Item::Shop_Item(Shop_Item_Type it) {
  item_type = it;
  SetSize(MathUtil::PixelsToWorldUnits(32),
          MathUtil::PixelsToWorldUnits(32));
  switch ( it ) {
    case Shop_Item_Type::Attack_Speed:
      SetSprite("Images\\item_attack_speed.png");
      gold_cost = 50;
    break;
    case Shop_Item_Type::Damage:
      SetSprite("Images\\item_damage.png");
      gold_cost = 75;
    break;
    case Shop_Item_Type::Health:
      SetSprite("Images\\item_health.png");
      gold_cost = 25;
    break;
    case Shop_Item_Type::Jump:
      SetSprite("Images\\item_jump.png");
      gold_cost = 50;
    break;
    case Shop_Item_Type::Walk_Speed:
      SetSprite("Images\\item_walk_speed.png");
      gold_cost = 80;
    break;
  }
  this->SetIsSensor(1);
  gold_show = new TextActor();
  theWorld.Add(gold_show);
  gold_show->SetColor(Color(.4,.7,.3));
  gold_show->SetDisplayString(std::to_string(gold_cost));
}
void Augments::Shop_Item::Update(float t) {
  gold_show->SetPosition(GetPosition().X-0.5,GetPosition().Y-2);
  if ( this->GetBoundingBox().Intersects(Game::thePlayer->GetBoundingBox()) ) {
    if ( theInput.IsKeyDown(GLFW_KEY_SPACE) ) {
      if ( 1/*gold_cost <= Game::thePlayer->R_Gold()*/ ) {
        Game::thePlayer->Add_Gold(-gold_cost);
        Destroy();
        for ( int i = 0; i != 3; ++ i )
          if ( Game::theKeep->items[i] == this ) {
            Game::theKeep->items[i] = nullptr; break;
          }
      }
    }
  }
}

Augments::Shop_Item::~Shop_Item() {
  gold_show->Destroy();
}