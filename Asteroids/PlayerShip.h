#pragma once

#include "GameObject.h"

class PlayerShip : public GameObject
{
public:
	std::vector<Vector3*> collisionVertices;

	PlayerShip( Vector3 position = Vector3::Zero(), Vector3 rotation = Vector3::Zero(), Color color = Color( 69, 206, 237 ) );

	void IncreaseVelocity( float amount );

	virtual void Draw( bool leftThruster, bool rightThruster ) const;
	virtual void FixedUpdate() override;

	virtual ~PlayerShip();

protected:
	virtual void ActiveDraw( bool leftThruster, bool rightThruster ) const;
};
