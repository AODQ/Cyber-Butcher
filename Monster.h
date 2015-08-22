#include "Augments.h"
#include "Angel.h"
#include "LD33.h"
#include "utility.h"
#ifndef MONSTER_H_
#define MONSTER_H_


namespace Player {
  // contains information like health attack etc
  // but more important body types
  class Monster : public PhysicsActor {
    int max_health, curr_health;
    int attack_damage;
    Augments::Weapon* frame_weapon;
    bool direction;

    float attack_cooldown;
    float special_cooldown;

    // physics
    int phys_jump_timer, phys_jump_timer_max;
  public:
    void Update(float dt);
    
    int R_Max_Health() const;
    int R_Curr_Health() const;
    int R_Attack_Damage() const;
    bool R_Direction() const;
    
    Augments::Weapon* R_Frame_Weapon();
    
    
    void Set_Max_Health(int);
    void Set_Curr_Health(int);
    void Add_Curr_Health(int);
    void Set_Attack_Damage(int);
    // will automatically remove old
    // actors from the stage and deallocate
    void Set_Frame_Weapon(Augments::Weapon_Type);
    
    
    Monster(Augments::Weapon_Type);
  };
  
};

using PlMonster = Player::Monster;


#endif