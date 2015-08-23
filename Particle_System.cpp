#include "stdafx.h"
#include "Angel.h"
#include "Hero.h"
#include <utility>
#include "Particle_System.h"

void Particles::Add_Bleed(const Vec2i Position, float angle) {
	float bleed_time = (float)theTuning.GetFloat("BleedLifeTime");

	ParticleActor* pa = new ParticleActor();
	bleed_particles.push_back(std::pair<ParticleActor*, float>(pa, bleed_time));

	theWorld.Add(pa);
	pa->SetParticleLifetime(bleed_time);
	pa->SetPosition(Position);
	pa->SetMaxParticles(100);
	pa->SetColor(Color(0.74f, 0.0f, 0.0f, 0.9f));
	pa->SetEndColor(Color(0.74f, 0.0f, 0.0f, 0.0f));
	pa->SetSize(Vector2(.1, .1));
	pa->SetSpread(angle);
	pa->SetParticlesPerSecond(3000);
	pa->ChangeColorTo(Color(0.74f, 0.0f, 0.0f, 0.0f), bleed_time);
}

Particles::Bleed_Container Particles::bleed_particles;


void Particles::Update(float dt) {
	for (int i = 0; i < bleed_particles.size(); ++i) {
		bleed_particles[i].second -= dt;
		if (bleed_particles[i].second <= 0) { // ran out of time remove
			bleed_particles[i].first->Destroy();
			bleed_particles.erase(bleed_particles.begin() + i, bleed_particles.begin() + i + 1);
		}
	}
}