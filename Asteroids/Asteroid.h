#pragma once

#include "GameObject.h"

class Asteroid : public GameObject
{
public:
	enum AsteroidSize { Large, Medium, Small };

	Asteroid( AsteroidSize size, Vector3 position = Vector3::Zero(), Vector3 rotation = Vector3::Zero(), Color color = Color( Utility::Random( 240 - 15, 240 + 15 ), Utility::Random( 138 - 15, 138 + 15 ), Utility::Random( 115 - 15, 115 + 15 ) ) );
	~Asteroid();
	float radius, radiusSquared; // The average radius of the asteroid, used for collision detection
	AsteroidSize size;

protected:
	int verticalVertices, horizontalVertices;
	float protrudeFactor;

	Vector3** vertices;

	virtual void ActiveDraw() const override;
};