#include "stdafx.h"
#include "Augments.h"
#include "Level.h"
#include "Angel.h"
#include "utility.h"
#include <vector>

void Level::Initialize() {
  //theWorld.SetSideBlockers(true, 0.0f);
  auto t = new Platform();
  t->SetPosition(Vector2(MathUtil::ScreenToWorld(0,
                                                 utility::Window_height)));
  t->SetSize(250,1);
  t->SetColor(0,0,0);
  theWorld.Add(t);
  t->InitPhysics();
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



extern Actor* Level::BG_Scroll::bg_scroll1 = nullptr,* Level::BG_Scroll::bg_scroll2 = nullptr;
extern float Level::BG_Scroll::clamp = 0;

void Level::BG_Scroll::Update(float dt) {
  
  if ( bg_scroll1 != nullptr && bg_scroll2 != nullptr ) {
    
    clamp += dt*0.3;
    bg_scroll1->SetPosition(clamp-10,0);
    bg_scroll2->SetPosition(clamp-60.f,0);
    if ( clamp > 50 ) clamp = 0;
  }
}

std::vector<Level::Platform*> Level::platforms;