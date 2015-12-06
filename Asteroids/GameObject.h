#pragma once

#ifdef _M_IX86
#include <windows.h>
#else
#include <stream.h>
#endif

class Rigidbody; // Forward declaration to prevent circular dependencies
#include "Rigidbody.h"
#include "Utility.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

class GameObject
{
public:
	float spawnTime;
	Vector3 position, rotation;
	Color color;

	Rigidbody* rigidbody;

	GameObject( Vector3 position = Vector3::Zero(), Vector3 rotation = Vector3::Zero(), Color color = Color::White() );

	void Draw() const;
	void Rotate( float aroundX, float aroundY, float aroundZ );
	virtual void Translate( float inX, float inY, float inZ );
	void Translate( Vector3 translation );
	Vector3 Forward();

	static void DefineNormal( Vector3 a, Vector3 b, Vector3 c );

	// This function will call FrameUpdate on each of the GameObject's Components
	virtual void FrameUpdate();

	// This function will call FixedUpdate on each of the GameObject's Components
	virtual void FixedUpdate();

	virtual ~GameObject();

protected:
	std::vector<GameObject*> children;

	virtual void PreDraw() const;
	virtual void ActiveDraw() const;
	virtual void PostDraw() const;
};
