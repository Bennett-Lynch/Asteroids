#include "GameObject.h"
#include "Globals.h"
#include <iostream>

GameObject::GameObject( Vector3 position, Vector3 rotation, Color color )
{
	spawnTime = ProgramTime();
	rigidbody = new Rigidbody( this );

	this->position = position;
	this->rotation = rotation;
	this->color = color;
}

// Call all 3 (virtual) functions to render the game object on screen
void GameObject::Draw() const
{
	PreDraw();
	ActiveDraw();
	PostDraw();

	for ( auto &c : children )
	{
		c->Draw();
	}
}

// Rotate the game object
void GameObject::Rotate( float aroundX, float aroundY, float aroundZ )
{
	rotation.x = Utility::PositiveMod( rotation.x + aroundX, 360.0f );
	rotation.y = Utility::PositiveMod( rotation.y + aroundY, 360.0f );
	rotation.z = Utility::PositiveMod( rotation.z + aroundZ, 360.0f );
}

// Translate the game object
void GameObject::Translate( float inX, float inY, float inZ )
{
	position = position.Translate( inX, inY, inZ );

	// If object moves off screen, move it to the opposite side
	if ( position.x < -( halfVisibleX + screenBorder ) )
	{
		position.x = halfVisibleX + screenBorder;
	}
	else if ( position.x >  halfVisibleX + screenBorder )
	{
		position.x = -( halfVisibleX + screenBorder );
	}

	if ( position.y < -( halfVisibleY + screenBorder ) )
	{
		position.y = halfVisibleY + screenBorder;
	}
	else if ( position.y >  halfVisibleY + screenBorder )
	{
		position.y = -( halfVisibleY + screenBorder );
	}
}

void GameObject::Translate( Vector3 translation )
{
	Translate( translation.x, translation.y, translation.z );
}

Vector3 GameObject::Forward()
{
	return Vector3( -sinf( Utility::DegreesToRadians( rotation.z ) ), cosf( Utility::DegreesToRadians( rotation.z ) ) );
}

// Setup the OpenGL drawing conditions prior to rendering every game object
void GameObject::PreDraw() const
{
	// Begin working on a new (temporary) transform matrix
	glPushMatrix();

	float colorArray[4] = { color.red, color.green, color.blue, color.alpha };
	glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colorArray );

	glTranslatef( position.x, position.y, position.z );

	glRotatef( rotation.x, 1, 0, 0 );
	glRotatef( rotation.y, 0, 1, 0 );
	glRotatef( rotation.z, 0, 0, 1 );
}

// The custom draw function that should be overriden by every game object
void GameObject::ActiveDraw() const
{
}

// Cleanup any temporary OpenGL conditions
void GameObject::PostDraw() const
{
	glPopMatrix(); // Remove the temporary transform matrix and return to our previous behavior
}

void GameObject::DefineNormal( Vector3 a, Vector3 b, Vector3 c )
{
	Vector3 ab = b - a;
	Vector3 bc = c - b;

	Vector3 cross = Vector3::Cross( ab, bc );
	float crossAray[3] = { cross.x, cross.y, cross.z };
	glNormal3fv( crossAray );
}

void GameObject::FrameUpdate()
{
	rigidbody->FrameUpdate();

	for ( auto &c : children )
	{
		c->FrameUpdate();
	}
}

void GameObject::FixedUpdate()
{
	rigidbody->FixedUpdate();

	for ( auto &c : children )
	{
		c->FixedUpdate();
	}
}

GameObject::~GameObject()
{
	for ( auto &c : children )
	{
		delete c;
	}
}