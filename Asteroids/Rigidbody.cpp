#include "Rigidbody.h"
#include "Globals.h"

Rigidbody::Rigidbody( GameObject* parentGameObject, Vector3 velocity, Vector3 angularVelocity )
{
	this->parentGameObject = parentGameObject;
	this->velocity = velocity;
	this->angularVelocity = angularVelocity;
}

void Rigidbody::FrameUpdate()
{
}

void Rigidbody::FixedUpdate()
{
	parentGameObject->Translate( velocity.x * fixedDeltaTime, velocity.y * fixedDeltaTime, velocity.z * fixedDeltaTime );
	parentGameObject->Rotate( angularVelocity.x * fixedDeltaTime, angularVelocity.y * fixedDeltaTime, angularVelocity.z * fixedDeltaTime );
}

Rigidbody::~Rigidbody()
{
}