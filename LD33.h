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
  extern bool controls_open;

  extern Player::Monster* thePlayer;

  extern bool input_polling;
  extern int key_polled;

  extern int right_key;
  extern int left_key;
  extern int punch_key;
  extern int jump_key;
  
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
  public:
    Actor* title_logo,
         * menu_background,
         * menu_start,
         * menu_controls,
         * menu_exit,
         * right_key,
         * left_key,
         * punch_key,
         * jump_key,
         * back_arrow,
         * change_key;

    void ChangeKey(int key);

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