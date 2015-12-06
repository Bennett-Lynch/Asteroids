#include "AlienShip.h"
#include "Rigidbody.h"
#include "Globals.h"
#include <iostream>

AlienShip::AlienShip( AlienSize size, Vector3 position, Vector3 rotation, Color color ) : GameObject( position, rotation, color )
{
	this->size = size;

	radius = 35;
	radiusSquared = pow( radius, 2 );

	if ( size == Small )
	{
		radius *= 0.65f;
		radiusSquared *= 0.65f;
	}

	// Add a new Rigidbody component to this object and give it a random velocity
	rigidbody->angularVelocity.z = 60.0f * ( Utility::RandomBool() ? 1 : -1 ) * ( size == Small ? 2.0f : 1.0f );
}

void AlienShip::ActiveDraw() const
{
	glScalef( radius, radius, radius );

	float wholeShipRadius = 1;
	float torusWidth = .25;
	glutSolidTorus( torusWidth, wholeShipRadius, 10, 10 );

	GLdouble eqn0[4] = { 0, 0, 1, 0 }; // Equation of plane, where 4th param moves plane
	glClipPlane( GL_CLIP_PLANE0, eqn0 );
	glEnable( GL_CLIP_PLANE0 );
	float sphereradius = wholeShipRadius - torusWidth;
	glutSolidSphere( sphereradius, 10, 10 );
	glDisable( GL_CLIP_PLANE0 );

	// Draw 8 miniature spheres around the ship for decoration
	glTranslatef( 0, 0, torusWidth );
	for ( int i = 0; i < 8; ++i )
	{
		glPushMatrix();

		float theta = ( i / 8.0f ) * static_cast<float>( 2 * M_PI );

		glTranslatef( sphereradius * cosf( theta ), sphereradius * sinf( theta ), 0 );

		glutSolidSphere( sphereradius * .15, 4, 4 );

		glPopMatrix();
	}
}

AlienShip::~AlienShip()
{
}

void AlienShip::FixedUpdate()
{
	// Change the y-velocity every few seconds
	if ( ( ProgramTime() > nextVelocityChangeTime ) )
	{
		rigidbody->velocity.y = Utility::Random( 50.0f, 100.0f ) * ( Utility::RandomBool() ? 1 : -1 ) * ( size == Small ? 2.0f : 1.0f );
		nextVelocityChangeTime = ProgramTime() + Utility::Random( 3.0f, 4.0f );
	}

	GameObject::FixedUpdate();
}

void AlienShip::Translate( float inX, float inY, float inZ )
{
	position = position.Translate( inX, inY, inZ );

	// We are intentionally not going to wrap the object around the screen in the x direction for the playership (so that Main.cpp can detect it and destroy it)

	if ( position.y < -( halfVisibleY + screenBorder ) )
	{
		position.y = halfVisibleY + screenBorder;
	}
	else if ( position.y >  halfVisibleY + screenBorder )
	{
		position.y = -( halfVisibleY + screenBorder );
	}
}