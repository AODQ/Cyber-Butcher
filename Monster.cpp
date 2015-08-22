#include "Monster.h"
#include "Augments.h"

namespace Player {


  void Monster::Refresh(float dt) {
    
    
  };


  int Monster::R_Max_Health()    const { return max_health; }
  int Monster::R_Curr_Health()   const { return curr_health; }
  int Monster::R_Attack_Damage() const { return attack_damage; }

  Head*   Monster::R_Frame_Head()   const { return frame_head; }
  Body*   Monster::R_Frame_Body()   const { return frame_body; }
  Weapon* Monster::R_Frame_Weapon() const { return frame_weapon; }


  void Monster::Set_Max_Health(int x)  { max_health = x; }
  void Monster::Set_Curr_Health(int x) { curr_health = x; }
  void Monster::Add_Curr_Health(int x) { curr_health += x; }
  void Monster::Set_Attack_Damage(int x) { attack_damage = x; }

  void Set_Frame_Head(Head_Type::HEAD_TYPE x) {
    if ( frame_head != nullptr ) {
      theWorld.Remove(frame_head);
      delete frame_head;
      frame_head = nullptr;
    }
    frame_head = new Head(x);
    theWorld.Add(x);
  };
  void Set_Frame_Weapon(Weapon_Type::WEAPON_TYPE x) {
    if ( frame_weapon != nullptr ) {
      theWorld.Remove(frame_weapon);
      delete frame_weapon;
      frame_weapon = nullptr;
    }
    frame_weapon = new Weapon(x);
    theWorld.Add(x);
  };
  void Set_Frame_Body(Body_Type::BODY_TYPE x) {
    if ( frame_body != nullptr ) {
      theWorld.Remove(frame_body);
      delete frame_body;
      frame_body = nullptr;
    }
    Body = new Body(x);
    theWorld.Add(x);
  };
  
  
  Monster(Head_Type::HEAD_TYPE head,
          Weapon_Type::WEAPON_TYPE weapon,
          Body_Type::BODY_TYPE body) {
    frame_body = frame_head = frame_weapon = nullptr;
    Set_Frame_Body((body));
    Set_Frame_Head(head);
    Set_Frame_Weapon((weapon));
  }
};