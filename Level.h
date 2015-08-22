#ifndef LEVEL_H_
#define LEVEL_H_


namespace Level {
  void Initialize(); // sets up walls
  void Clear(); // clears level
  
  std::vector<Platform*> platforms;
  
  class Platform : public PhysicsActor {
    Platform();
  }
}



#endif