#include "stdafx.h"
#include "Augments.h"
#include "Level.h"
#include "Angel.h"
#include <vector>

void Level::Initialize() {
  theWorld.SetSideBlockers(true, 0.0f);
}
  
void Level::Clear() {
  for(int i = 0; i < platforms.size(); i++) {
    platforms[i]->Destroy();
  }
    
  platforms.clear();
}
  
Level::Platform::Platform() {
  SetFriction(0.6f);
  SetDensity(0.0f);
  SetRestitution(0.0f);
}

std::vector<Level::Platform*> Level::platforms;