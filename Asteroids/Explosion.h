#pragma once
#include "GameObject.h"
#include "Particle.h"
#include "Utility.h"

class Explosion : public GameObject
{
public:
	const float lifeTime = 1.25f;

	Explosion( int numParticles, float particleScalar, Vector3 position = Vector3::Zero(), Vector3 rotation = Vector3::Zero(), Color color = Color::White() );

	virtual void FixedUpdate() override;
};
