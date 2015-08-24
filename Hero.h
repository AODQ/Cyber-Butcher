#ifndef HERO_H_
#define HERO_H_

#include "Angel.h"
#include "LD33.h"
#include "Level.h"
#include <string>

namespace Hero {

  // abilities used by enemy heroes
  enum class Weapon { // 
    sword
  };

  enum class Throwable { // 
    dagger
  };

  class Enemy : public PhysicsActor {
    bool intro;
    float intro_time;
    float speed;
    int health;
    float slide_cooldown;
    std::string name;
    float movement_cooldown, melee_cooldown, range_cooldown;
    float movement_attack_flinch; // stops movement altogether after an attack

    float slide_timer;
    bool slide_direction;
    float ghost_timer;
    float ghost_cooldown;
    float platform_cooldown;
    float on_platform_timer;
    bool jumping_to_platform;    // hero is jumping up on top of the platform

    Level::Platform* platform;


    enum class Mood { // mood for how far they want to be
      Close, // only melee
      Fair, // could probably do a mix of ranged and melee
      Distant, // only ranged
      Size
    };

    Mood mood;
    TextActor* mood_tester;

    float mood_switch_timer;
    float jump_timer; // every 5 seconds we get a chance to jump
    float in_air_start, in_air_end; // used by attack. attack after start before end

    // takes care of cooldowns
    void Attack_Melee();
    // takes care of cooldowns
    void Attack_Range();

    void Apply_Vel_X(float x, float d);
  public:
    Enemy();
    void Update(float dt);
    void Jump();
    inline void Add_Health(int x) { health += x; }
	  void Killed();
  };

  /*// just moves into the screen all cool and then spawns Enemy
  class Enemy_Intro : public Actor {
  public:
    Enemy_Intro();
  };*/

  // used for enemy AI
  class Enemy_Listener : public MessageListener {
  public:
    void ReceiveMessage(Message* );
  };

  extern Enemy* theEnemy;
  //extern Enemy_Intro* theEnemyIntro;
  extern Enemy_Listener* e_listener;


  // enemy attacks
  class Dagger : public PhysicsActor {
    float lifetime;
  public:
    Dagger(float angle, Vec2i position);
    void Update(float dt);
  };

  class Gold : public PhysicsActor {
    float amount;
  public:
	  Gold(Vector2 position);
    void Update(float dt);
  };
};

#endif