#ifndef LD33_H_
#define LD33_H_

#include "Angel.h"
#include "Augments.h"
#include "Monster.h"

// main game stuff goes here
namespace Game {
  void Initialize();
  
  void Clear();
  
  extern Player::Monster* thePlayer;
  
  
  class Mouse : public MouseListener {
  public:
    void MouseDownEvent(Vec2i, MouseButtonInput);
  };
  
};

#endif