#include "stdafx.h"
#include "Augments.h"
#include "Level.h"
#include "Angel.h"
#include <vector>

void Level::Initialize() {
  theWorld.SetSideBlockers(true, 0.0f);
    
    
  /*Platform* test_platform = new Platform();
  theWorld.Add(test_platform);
  platforms.push_back(test_platform);
  test_platform->SetPosition(0.0f, 0.0f);
  test_platform->SetSize(50.0f, 5.0f);
  test_platform->SetDrawShape(ADS_Square);
  test_platform->SetColor(0, 0, 0);*/
}
  
void Level::Clear() {
  for(int i = 0; i < platforms.size(); i++) {
    theWorld.Remove(platforms[i]);
    delete platforms[i];
  }
    
  platforms.clear();
}
  
Level::Platform::Platform() {
  SetFriction(0.6f);
  SetDensity(0.0f);
  SetRestitution(0.0f);
  InitPhysics();
}

std::vector<Level::Platform*> Level::platforms;