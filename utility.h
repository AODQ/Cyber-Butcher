#include <random>
#include "stdafx.h"
#include <typeindex>

#ifndef __UTILITY_H_
#define __UTILITY_H_

// general utility purposed things

namespace utility {
  /* rand eng stuff*/
  extern std::random_device rand;
  extern std::mt19937 rgen;
  extern std::uniform_real_distribution<> dist;

  extern const int Window_width;
  extern const int Window_height;
  
  // returns a random float from 0 to 100
  float R_Rand();
  
  // output to console (primarily for debugging purposes)
  void Output(std::string);
  
  template <typename T>
  T R_Max(T x, T y);
  
  template <typename T>
  T R_Min(T x, T y);
  
  
  bool Find_Substr(std::string&, std::string);
  template <typename T>
  T Str_To_Type(std::string x);
  
  int GL_To_Angel(int);

  /*// doesn't work
  
  // for use by collision_entity. Means absolutely nothing. Used as a default
  // 'not colliding' state.
  class nil {};

  // For grabbing contact of collisions
  // Everything that collides and needs to know what
  // it collides with /MUST/ use this! Be sure to
  // set contact ID to -1 every frame.
  class Collision_Entity {
  public:
    enum class Type {
      nil, monster, hero, platform, dagger
    };
  protected:
    Type contact_type;
  public:
    virtual Type R_Entity_Type() = 0;
    inline void Set_Contact_Type(Type T) { contact_type = T; }
  };

  // used for collisions that primarily deal damage
  class Contact_Listener : public b2ContactListener {
    void BeginContact(b2Contact* contact);
    void EndContact(  b2Contact* contact);
  };
  //extern int mouse_x, mouse_y;*/
};

#endif