#include "Level.h"

namespace Level {
  void Initialize() {
    theWorld::SetSideBlockers(true, 0.0f);
    
    /*Level::Platform left_wall;
    Level::Platform right_wall;
    Level::Platform top_wall;
    Level::Platform bottom_wall;*/
    
    Platform* test_platform = new Platform();
    theWorld.Add(test_platform);
    platforms.push_back(test_platform);
    test_platform->SetPosition(0.0f, 0.0f);
    test_platform->SetSize(50.0f, 5.0f);
    test_platform->SetDrawShape(ADS_Square);
    test_platform->SetColor(0, 0, 0);
  }
  
  void Clear() {
    for(int i = 0; i < platforms.size(); i++) {
      theWorld.Remove(platforms[i]);
      delete platforms[i];
    }
    
    platforms.clear();
  }
  
  Platform:Platform() {
    SetFriction(0.6f);
    SetDensity(0.0f);
    SetRestitution(0.0f);
    InitPhysics();
  }
}