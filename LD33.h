#ifndef LD33_H_
#define LD33_H_

#include "Angel.h"
#include "Augments.h"
#include "Monster.h"
#include "utility.h"
#include <typeinfo>

// main game stuff goes here
namespace Game {
  void Initialize(); // for menu
  void Initialize_Game(); // rest
  
  void Clear();

  extern bool in_menu;

  extern Player::Monster* thePlayer;
  
  class Mouse : public MouseListener {
  public:
    void MouseDownEvent(Vec2i, MouseButtonInput);
  };
  
  // for general events. Just does whatever it feels like.
  class Overseer : public Actor {
    Actor* menu_start,
         * menu_controls,
         * menu_exit,
         * menu_select;
    int selected_icon;
  public:
    int level;
    void Start_Game();
    Overseer();
    void Update(float dt);
  };

  extern Overseer* theOverseer;
  extern Augments::ShopKeep* theKeep;

};

#endif