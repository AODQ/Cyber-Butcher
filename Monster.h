  // monster related
#ifndef MONSTER_H_
#define MONSTER_H_

#include "Augments.h"
  
namespace Player {
  
  class Weapon;
  class Head;
  class Body;
  

  
  
  // contains information like health attack etc
  // but more important body types
  class Monster : public PhysicsActor {
    int max_health, curr_health;
    int attack_damage;
    Head* frame_head;
    Weapon* frame_weapon;
    Body* frame_body;
  public:
    void Refresh(float dt);
    
    int R_Max_Health() const;
    int R_Curr_Health() const;
    int R_Attack_Damage() const;
    
    Head* R_Frame_Head();
    Weapon* R_Frame_Weapon();
    Body* R_Frame_Body();
    
    
    void Set_Max_Health(int);
    void Set_Curr_Health(int);
    void Add_Curr_Health(int);
    void Set_Attack_Damage(int);
    // will automatically remove old
    // actors from the stage and deallocate
    void Set_Frame_Head(Head_Type::HEAD_TYPE);
    void Set_Frame_Weapon(Weapon_Type::WEAPON_TYPE);
    void Set_Frame_Body(Body_Type::BODY_TYPE);
    
    
    Monster(Head_Type::HEAD_TYPE,
            Weapon_Type::WEAPON_TYPE,
            Body_Type::BODY_TYPE);
  };
  
};


#endif