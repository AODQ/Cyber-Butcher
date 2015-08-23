#ifndef HERO_H_
#define HERO_H_

#include "Angel.h"
#include "LD33.h"

namespace Hero {

  // abilities used by enemy heroes
  enum class Ability {
    dagger_throw, slide

  };
  class Enemy : public PhysicsActor {
    bool intro;
    float intro_time;
    float speed;
    Vector2 target_location;
    int health;
    int ab1_cooldown, ab2_cooldown;
    Ability ab1, ab2; // 2 for now, maybe 3 in future? shrug
  public:
    Enemy();
    void Update(float dt);
    void Jump();
    virtual void DecideTarget();
    inline void Add_Health(int x) { health += x; }
	  void Killed();
  };

  void Cast_Ability(Ability);

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