#ifndef PARTICLE_SYSTEM_H_
#define PARTICLE_SYSTEM_H_

#include "Angel.h"
#include <utility>

namespace Particles {
  void Add_Bleed(const Vec2i Position, float angle, int max_particles = 0);

  using Bleed_Container = std::vector<std::pair<ParticleActor*,float>>;

  extern Bleed_Container bleed_particles;

  void Update(float dt);
};


#endif