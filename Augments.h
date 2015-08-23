#ifndef AUGMENTS_H_
#define AUGMENTS_H_

#include "Angel.h"

namespace Augments {
  // only used for pass-ins for constructors
  
  enum class Weapon_Type {
    Big_Sword
  };
  
  enum class Shop_Item_Type {
    Health, Damage, Jump, Walk_Speed, Attack_Speed, Size
  };
  
  class Shop_Item : public PhysicsActor {
    Shop_Item_Type item_type;
    int gold_cost;
    TextActor* gold_show;
  public:
    Shop_Item(Shop_Item_Type);
    ~Shop_Item();
    void Update(float t);
  };
  
  class Weapon : public Actor {
    const Weapon_Type type;
    float animation_timer;
    PhysicsActor* coll_check;
    bool origin_direction;
  public:
    Weapon(Weapon_Type);
    void Update(float dt);
    void Cast();
    inline const Weapon_Type R_Type() const { return type; }
  };

  class ShopKeep : public Actor {
    bool _active;
    Shop_Item* items[3];
    friend Shop_Item;
  public:
    float time_left;

    ShopKeep();
    void Update(float dx);
    void NewItems();

    inline bool R_Active() const { return _active; }
  };
}
using AugWep  = Augments::Weapon;

#endif