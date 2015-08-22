// monster related

#include "Augments.h"
#include "Angel.h"

#ifndef MONSTER_H_
#define MONSTER_H_

namespace Player {
  // contains information like health attack etc
  // but more important body types
  class Monster : public PhysicsActor {
    int max_health, curr_health;
    int attack_damage;
    Augments::Head* frame_head;
    Augments::Weapon* frame_weapon;
    Augments::Body* frame_body;
  public:
    void Refresh(float dt);
    
    int R_Max_Health() const;
    int R_Curr_Health() const;
    int R_Attack_Damage() const;
    
    Augments::Head* R_Frame_Head();
    Augments::Weapon* R_Frame_Weapon();
    Augments::Body* R_Frame_Body();
    
    
    void Set_Max_Health(int);
    void Set_Curr_Health(int);
    void Add_Curr_Health(int);
    void Set_Attack_Damage(int);
    // will automatically remove old
    // actors from the stage and deallocate
    void Set_Frame_Head(Augments::Head_Type);
    void Set_Frame_Weapon(Augments::Weapon_Type);
    void Set_Frame_Body(Augments::Body_Type);
    
    
    Monster(Augments::Head_Type,
            Augments::Weapon_Type,
            Augments::Body_Type);
  };
  
};

using PlMonster = Player::Monster;


#endif