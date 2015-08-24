#ifndef SOUNDS_H_
#define SOUNDS_H_

#include "Angel.h"

namespace Sounds {
  extern AngelSampleHandle Gibberish[36];
  extern AngelSampleHandle Inventory[17];
  extern AngelSampleHandle WTF[17];
  extern AngelSampleHandle Monster_Death;
  extern AngelSampleHandle boss_damaged,
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

  extern int gibberish_count; // amount of times gibberish has played
  extern AngelSoundHandle gibberish_handle;
  extern int monster_gibber_timer; // wait a second or so before monster responds

  void Load_Sounds();
  // includes WTF
  void Play_Gibberish();
  void Play_Inventory();
  void Play_Death();


  void Update(float dt);
};


#endif