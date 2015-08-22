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

  class Scroll_Background : public Actor {
    float clip, clip_max;
  public:
    Scroll_Background(int pixel_width);
    void Update(float dt);
  };
}



#endif