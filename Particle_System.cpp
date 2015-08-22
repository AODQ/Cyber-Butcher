#include "stdafx.h"
#include "Angel.h"
#include "Hero.h"
#include <utility>
#include "Particle_System.h"

void Particles::Add_Bleed(const Vec2i Position, float angle) {
  ParticleActor* pa = new ParticleActor();
  bleed_particles.push_back(std::pair<ParticleActor*,float>(pa,
            static_cast<float>(theTuning.GetInt("BleedLifeTime"))));
  theWorld.Add(pa);
  pa->SetParticleLifetime(1);
  pa->SetPosition(Position);
  pa->SetMaxParticles(50);
  pa->SetColor(Color(.3,.9,.9,.5));
  pa->SetSpread(angle);
  pa->SetParticlesPerSecond(40);
}

Particles::Bleed_Container Particles::bleed_particles;


void Particles::Update(float dt) {
  for ( int i = 0; i != bleed_particles.size(); ++ i ) {
    bleed_particles[i].second -= dt;
    if ( bleed_particles[i].second <= 0 ) { // ran out of time remove
      bleed_particles[i].first->Destroy();
      bleed_particles.erase(bleed_particles.begin()+i,bleed_particles.begin()+i+1);
    }
  }
}