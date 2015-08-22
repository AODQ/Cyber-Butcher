#ifndef AUGMENTS_H_
#define AUGMENTS_H_

namespace Augments {
    // only used for pass-ins for constructors
  namespace Weapon_Type {
    enum WEAPON_TYPE {
      sword
    };    
  };
  
  namespace Head_Type {
    enum HEAD_TYPE {
      head
    };
  };
  
  namespace Body_Type {
    enum BODY_TYPE{
      body
    };
  };
  
  
  // the player (Monster) has a head, body and weapon
  // that augments his abilities. This will mostly
  // be the thing that draws to the screen for each
  // augmentation, but more importantly it contains
  // specific statistics about each one.
  
  // So if there was a body that was able to fly,
  // there would be a bool that denotes this ability.
  
  class Head : public Actor {
    const Head_Type::HEAD_TYPE type;
  public:
    Head(Head_Type::HEAD_TYPE);
    void Update(float dt);
  };
  
  class Body : public Actor {
    const Body_Type::BODY_TYPE type;
  public:
    Body(Body_Type::BODY_TYPE);
    void Update(float dt);
  };
  
  class Weapon : public Actor {
    const Weapon_Type::WEAPON_TYPE type;
  public:
    Weapon(Weapon_Type::WEAPON_TYPE);
    void Update(float dt);
  };
};

#endif