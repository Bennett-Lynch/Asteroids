#pragma once

class GameObject; // Forward declaration to prevent circular dependencies
#include "GameObject.h"
#include "Utility.h"

class Rigidbody
{
public:
	GameObject* parentGameObject;

	Vector3 velocity, angularVelocity;

	Rigidbody( GameObject* parentGameObject, Vector3 velocity = Vector3::Zero(), Vector3 angularVelocity = Vector3::Zero() );

	void FrameUpdate();
	void FixedUpdate();

	~Rigidbody();
};
