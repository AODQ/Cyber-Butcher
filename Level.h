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
    // you need to initPhysics on your own!
    Platform();
  };

  extern std::vector<Platform*> platforms;

  namespace BG_Scroll {
    extern Actor* bg_scroll1, *bg_scroll2;
    extern float clamp;

    void Update(float dt);
  }

  namespace _HUD {
    extern Actor* hud_bottom,
                * hud_bitch,
                * hud_hero,
                * hud_monster;
    extern TextActor* hero_health,
                   *  monster_health;
    void Init();
    void Apply();
  };

  namespace Leaves {
    extern float wind_speed;
    extern float wind_count;
    
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
<<<<<<< HEAD

  /*namespace Inventory {
    extern float cooldown;


    enum class _Item {
      HP_Mini, HP_Reg, HP_Large,
      Mana_Mini, Mana_Reg, Mana_Large,
      Throw_Axe, Magic, Knife, Shuriken,
      Dagger, Long_Sword, Claymore, Axe, Battle_Axe, Uber_Axe, Spear,
      Amulet, Ring, Armor, Leggings, Shoes, size
    };

    Actor* items[14];
    Item   items_[14];

    Actor* amulet, *ring, *leggings, *shoes, *weapon, *throwable;

    void Load_Sprites();

    void Load_New_Inventory(int level);
    void Play_Inventory();
  }*/
}
=======
};
>>>>>>> 21508a30f2249ec0bea47e3bf4cb228d0095bb46



#endif