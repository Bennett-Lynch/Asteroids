#pragma once

#include "GameObject.h"

class AlienShip : public GameObject
{
public:
	enum AlienSize { Large, Small };
	AlienSize size;
	float radius, radiusSquared; // Used for collision detection

	AlienShip( AlienSize size, Vector3 position = Vector3::Zero(), Vector3 rotation = Vector3::Zero(), Color color = Color( 69, 237, 119 ) );
	virtual ~AlienShip();

	virtual void FixedUpdate() override;
	void Translate( float inX, float inY, float inZ ) override;

protected:
	float nextVelocityChangeTime;

	virtual void ActiveDraw() const override;
};
