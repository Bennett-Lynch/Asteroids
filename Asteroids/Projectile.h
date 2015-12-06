#pragma once

#include "GameObject.h"

class Projectile : public GameObject
{
public:
	const float lifeTime = 2.0f;
	float radius, radiusSquared; // Used for collision detection

	Projectile( Vector3 position = Vector3::Zero(), Vector3 rotation = Vector3::Zero(), Color color = Color::White() );
	~Projectile();

protected:
	virtual void ActiveDraw() const override;
};
