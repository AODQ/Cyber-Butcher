#ifndef LEVEL_H_
#define LEVEL_H_

#include <vector>
#include "Angel.h"
#include "utility.h"

namespace Level {
  void Initialize(); // sets up walls
  void Clear(); // clears level
  
  class Platform : public PhysicsActor {
  public:
    Platform();
  };

  extern std::vector<Platform*> platforms;

  namespace BG_Scroll {
    extern Actor* bg_scroll1, *bg_scroll2;
    extern float clamp;

    void Update(float dt);
  }

  namespace Leaves {
    extern float wind_speed;
    
    
    class Leaf : public PhysicsActor {
      float transparency;
      float force;
      float lifetime;
      float dip_up;
      float dip_down;
      float anim_speed;
      int anims, curr_anim;
    public:
      Leaf();
      void Update(float);
    };

    extern float rand_until_next_leave;

    void Update(float);
  };
}



#endif