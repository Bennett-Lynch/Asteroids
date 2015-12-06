#pragma once
#include "GameObject.h"
class Particle : public GameObject
{
public:
	float scale;
	Particle( float scale, Vector3 position = Vector3::Zero(), Vector3 rotation = Vector3::Zero(), Color color = Color( 255, Utility::Random( 128, 255 ), 0 ) );

protected:
	virtual void ActiveDraw() const override;
};