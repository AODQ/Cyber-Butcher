#ifndef HERO_H_
#define HERO_H_

#include "Angel.h"

namespace Hero {

  // abilities used by enemy heroes
  enum class Ability {
    dagger_throw, slide

  };
  class Enemy : public PhysicsActor {
    int health;
    Ability ab1, ab2; // 2 for now, maybe 3 in future? shrug
  public:
    Enemy();
    void Update(float dt);
  };

  void Cast_Ability(Ability, Enemy*);

  // just moves into the screen all cool and then spawns Enemy
  class Enemy_Intro : public Actor {
  public:
    Enemy_Intro();
  };

  // used for enemy AI
  class Enemy_Listener : public MessageListener {
  public:
    void ReceiveMessage(Message* );
  };

  extern Enemy* theEnemy;
  extern Enemy_Intro* theEnemyIntro;
  extern Enemy_Listener* e_listener;


  // enemy attacks
  class Dagger : public PhysicsActor {
    float lifetime;
  public:
    Dagger(float angle, Vec2i position);
    void Update(float dt);
  };
};

#endif