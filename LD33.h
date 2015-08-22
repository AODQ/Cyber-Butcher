#include "Angel.h"
#include "Augments.h"
#include "Monster.cpp"

// main game stuff goes here
namespace Game {
  Initialize();
  
  Clear();
  
  extern Player::Monster* thePlayer;
  
  
  class Mouse : public MouseListener {
  public:
    void MouseDownEvent(Vec2i, MouseButtonImput);
  };
  
};