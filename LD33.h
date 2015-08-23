#ifndef LD33_H_
#define LD33_H_

#include "Angel.h"
#include "Augments.h"
#include "Monster.h"
#include "utility.h"
#include <typeinfo>

// main game stuff goes here
namespace Game {
  void Initialize();
  
  void Clear();


  extern Player::Monster* thePlayer;
  
  class Mouse : public MouseListener {
  public:
    void MouseDownEvent(Vec2i, MouseButtonInput);
  };
  
  // for general events. Just does whatever it feels like.
  class Overseer : public Actor {
    bool first_enemy;
  public:
    Overseer();
    void Update(float dt);
  };

  extern Overseer* theOverseer;

};

#endif