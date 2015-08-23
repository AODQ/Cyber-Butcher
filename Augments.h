#ifndef AUGMENTS_H_
#define AUGMENTS_H_

#include "Angel.h"

namespace Augments {
  // only used for pass-ins for constructors
  
  enum class Weapon_Type {
    Big_Sword
  };
  // the player (Monster) has a head, body and weapon
  // that augments his abilities. This will mostly
  // be the thing that draws to the screen for each
  // augmentation, but more importantly it contains
  // specific statistics about each one.
  
  // So if there was a body that was able to fly,
  // there would be a bool that denotes this ability.
  
  
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