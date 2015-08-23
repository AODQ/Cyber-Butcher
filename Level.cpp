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

namespace LLeaves = Level::Leaves;

float LLeaves::rand_until_next_leave = -1;
float LLeaves::wind_speed = 0;

void LLeaves::Update(float t) {
  if ( rand_until_next_leave == -1 ) {
    auto t = new LLeaves::Leaf();
    theWorld.Add(t);
    rand_until_next_leave = utility::R_Rand()/25+1;
    wind_speed = utility::R_Rand()-50;
  }
  rand_until_next_leave -= t;
  if ( rand_until_next_leave - t <= 0 ) {
    rand_until_next_leave = -1; // gen next one
    int amount_of_leaves = int(utility::R_Rand())%2+1;
    while ( --amount_of_leaves != -1 ) {
      auto t = new LLeaves::Leaf();
      
      theWorld.Add(t);
    }
  }
}


LLeaves::Leaf::Leaf() {
  //this->SetFriction(0.8);
  //this->SetRestitution(0.0);
  anim_speed = 0.6;
  this->SetPosition(MathUtil::ScreenToWorld(20,20));
  this->SetSize(MathUtil::PixelsToWorldUnits(8),
                MathUtil::PixelsToWorldUnits(8));
  this->SetDrawShape(ADS_Square);
  dip_up = 0;
  curr_anim = 0;
  //return;
  switch( int(utility::R_Rand())%8 ) {
    case 0:
      this->LoadSpriteFrames("Images\\leaf0r_000.png");
      anims = 6;
    break;
    case 1:
      this->LoadSpriteFrames("Images\\leaf1r_000.png");
      anims = 6;
    break;
    case 2:
      this->LoadSpriteFrames("Images\\leaf2r_000.png");
      anims = 8;
    break;
    case 3:
      this->LoadSpriteFrames("Images\\leaf3r_000.png");
      anims = 12;
    break;
    case 4:
      this->LoadSpriteFrames("Images\\leaf0l_000.png");
      anims = 6;
    break;
    case 5:
      this->LoadSpriteFrames("Images\\leaf1l_001.png");
      anims = 6;
    break;
    case 6:
      this->LoadSpriteFrames("Images\\leaf2l_000.png");
      anims = 8;
    break;
    case 7:
      this->LoadSpriteFrames("Images\\leaf3l_000.png");
      anims = 12;
    break;
  }
  this->InitPhysics();
  //this->GetBody()->SetGravityScale(0.1f);
};


void LLeaves::Leaf::Update(float dt) {
  anim_speed -= dt;
  if ( anim_speed <= 0 ) {
    anim_speed = 0.2;
    curr_anim = (curr_anim+1)%anims;
    SetSpriteFrame(curr_anim);
    GetBody()->SetTransform(b2Vec2(0,0),0);
  }
  /*ApplyForce(Vector2(wind_speed/10,dip_up?-10:0),0);
  if ( dip_up  > 0 ) {
    dip_up -= dt;
  } else {
    if ( dip_up != -100 ) { // reset
      dip_up = -100;
      dip_down = utility::R_Rand()/50;
    } else {
      dip_down -= dt;
      if ( dip_down < 0 ) dip_up = utility::R_Rand()/50;
    }
  }*/
}