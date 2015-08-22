#ifndef AUGMENTS_H_
#define AUGMENTS_H_

#include "Angel.h"

namespace Augments {
  // only used for pass-ins for constructors
  enum class Weapon_Type {
    sword
  };

  enum class Head_Type {
    head
  };
  
  enum class Body_Type {
    body
  };
  
  // the player (Monster) has a head, body and weapon
  // that augments his abilities. This will mostly
  // be the thing that draws to the screen for each
  // augmentation, but more importantly it contains
  // specific statistics about each one.
  
  // So if there was a body that was able to fly,
  // there would be a bool that denotes this ability.
  
  class Head : public Actor {
    const Head_Type type;
  public:
    Head(Head_Type);
    void Update(float dt);
  };
  
  class Body : public Actor {
    const Body_Type type;
  public:
    Body(Body_Type);
    void Update(float dt);
  };
  
  class Weapon : public Actor {
    const Weapon_Type type;
  public:
    Weapon(Weapon_Type);
    void Update(float dt);
  };
}

using AugHead = Augments::Head;
using AugBody = Augments::Body;
using AugWep  = Augments::Weapon;

#endif