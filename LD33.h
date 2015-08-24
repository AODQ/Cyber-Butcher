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
    Vector2* mouse_position;
  public:
    Mouse();
    void MouseDownEvent(Vec2i, MouseButtonInput);
    void MouseMotionEvent(Vec2i);

    Vector2* R_Mouse_Position() const;
  };
  
  extern Mouse* theMouse;
  
  // for general events. Just does whatever it feels like.
  class Overseer : public Actor {
    Actor* menu_start,
         * menu_controls,
         * menu_exit,
         * menu_select,
         * selected_icon;
  public:
    Actor* pressed_button;
    int level;
    void Start_Game();
    Overseer();
    void Update(float dt);
  };

  extern Overseer* theOverseer;
  extern Augments::ShopKeep* theKeep;

};

#endif