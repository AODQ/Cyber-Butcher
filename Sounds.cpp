#include "stdafx.h"
#include "Angel.h"
#include "utility.h"
#include "Sounds.h"
#include "Hero.h"
#include "LD33.h"
  
AngelSampleHandle Sounds::Gibberish[36];
AngelSampleHandle Sounds::Inventory[17];
AngelSampleHandle Sounds::WTF[17];
AngelSampleHandle Sounds::Monster_Death;

namespace Sounds {
 AngelSampleHandle boss_damaged,
                           boss_explode,
                           boss_footstep,
                           boss_jump,
                           boss_land,
                           boss_punch,
                           boss_special,
                           hero_attack,
                           hero_damaged,
                           hero_death,
                           hero_jump,
                           hero_slide,
                           hero_throw,
                           menu_press,
                           menu_select,
                           teleport;

};

int Sounds::gibberish_count = 0;
AngelSoundHandle Sounds::gibberish_handle;
int Sounds::monster_gibber_timer = 0;

void Sounds::Load_Sounds() {
  for ( int i = 1; i != 18; ++ i ) {
    std::string file_name = "Sounds\\Boss\\WTF\\wtf" + std::to_string(i) + ".ogg";
    WTF[i-1] = theSound.LoadSample(file_name,1);
    file_name = "Sounds\\Inventory\\inv" + std::to_string(i) + ".ogg";
    Inventory[i-1] = theSound.LoadSample(file_name,1);
  }
    
  for ( int i = 1; i != 36; ++ i ) {
    std::string file_name = "Sounds\\Hero\\word" + std::to_string(i) + ".ogg";
    Gibberish[i-1] = theSound.LoadSample(file_name,1);
  }

  Monster_Death = theSound.LoadSample("Sounds\\Boss\\Death.ogg", 1);

  boss_damaged   = theSound.LoadSample("Sounds\\Effects\\boss-damaged.ogg", 1);
  boss_explode   = theSound.LoadSample("Sounds\\Effects\\boss-explode.ogg", 1);
  boss_footstep  = theSound.LoadSample("Sounds\\Effects\\boss-footstep.ogg", 1);
  boss_jump      = theSound.LoadSample("Sounds\\Effects\\boss-jump.ogg", 1);
  boss_land      = theSound.LoadSample("Sounds\\Effects\\boss-land.ogg", 1);
  boss_punch     = theSound.LoadSample("Sounds\\Effects\\boss-punch.ogg", 1);
  boss_special   = theSound.LoadSample("Sounds\\Effects\\boss-special.ogg", 1);
  hero_attack    = theSound.LoadSample("Sounds\\Effects\\hero-attack.ogg", 1);
  hero_damaged   = theSound.LoadSample("Sounds\\Effects\\hero-damaged.ogg", 1);
  hero_death     = theSound.LoadSample("Sounds\\Effects\\hero-death.ogg", 1);
  hero_jump      = theSound.LoadSample("Sounds\\Effects\\hero-jump.ogg", 1);
  hero_slide     = theSound.LoadSample("Sounds\\Effects\\hero-slide.ogg", 1);
  hero_throw     = theSound.LoadSample("Sounds\\Effects\\hero-throw.ogg", 1);
  menu_press     = theSound.LoadSample("Sounds\\Effects\\menu-press.ogg", 1);
  menu_select    = theSound.LoadSample("Sounds\\Effects\\menu-select.ogg", 1);
  teleport       = theSound.LoadSample("Sounds\\Effects\\teleport.ogg", 1);
}
// includes WTF
void Sounds::Play_Gibberish() {
  gibberish_count = 0;
  gibberish_handle = theSound.PlaySound(Gibberish[int(utility::R_Rand())%36]);
}
void Sounds::Play_Inventory() {
  theSound.PlaySound(Inventory[int(utility::R_Rand())%17]);
}
void Sounds::Play_Death() {
  theSound.PlaySound(Monster_Death);
}

void Sounds::Update(float dt) {
  if ( monster_gibber_timer > 0 ) {
    monster_gibber_timer -= dt;
    if ( monster_gibber_timer <= 0 ) {
      monster_gibber_timer = 0;
      theSound.PlaySound(WTF[int(utility::R_Rand())%17]);
    }
  }
  if ( gibberish_handle == nullptr ) return;
  if ( !theSound.IsPlaying(gibberish_handle) ) {
    gibberish_handle = nullptr;
    if ( Hero::theEnemy == nullptr ) // dead
      gibberish_count = 5; // skip
    if ( ++gibberish_count <= 2 ) {
      // if 1..2(50%) play another sound 
      if ( gibberish_count != 2 || (gibberish_count == 2 && utility::R_Rand()<50) ) {
        gibberish_handle = theSound.PlaySound(Gibberish[int(utility::R_Rand())%36]);
      }
    }

    if ( gibberish_handle == nullptr ) { // play WTF sound
      monster_gibber_timer = 1 + utility::R_Rand()/70;
    }
  }
}