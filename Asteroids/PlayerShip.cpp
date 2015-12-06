#include "PlayerShip.h"
#include "Globals.h"

PlayerShip::PlayerShip( Vector3 position, Vector3 rotation, Color color ) : GameObject( position, rotation, color )
{
	collisionVertices.push_back( new Vector3( 0, 38, 0 ) ); // nose
	collisionVertices.push_back( new Vector3( 14, 31, 0 ) );
	collisionVertices.push_back( new Vector3( 19, 11, 0 ) );
	collisionVertices.push_back( new Vector3( 31, 3, 0 ) );
	collisionVertices.push_back( new Vector3( 44, -5, 0 ) );
	collisionVertices.push_back( new Vector3( 59, -16, 0 ) ); // wing tip
	collisionVertices.push_back( new Vector3( 43, -14, 0 ) );
	collisionVertices.push_back( new Vector3( 28, -12, 0 ) );
	collisionVertices.push_back( new Vector3( 13, -15, 0 ) );

	// Extra vertices for alien projectile detection
	collisionVertices.push_back( new Vector3( 4, 32, 0 ) );
	collisionVertices.push_back( new Vector3( 7, 28, 0 ) );
	collisionVertices.push_back( new Vector3( 9, 31, 0 ) );
	collisionVertices.push_back( new Vector3( 14, 14, 0 ) );
	collisionVertices.push_back( new Vector3( 14, 20, 0 ) );
	collisionVertices.push_back( new Vector3( 14, 26, 0 ) );
	collisionVertices.push_back( new Vector3( 23, 9, 0 ) );
	collisionVertices.push_back( new Vector3( 27, 6, 0 ) );
	collisionVertices.push_back( new Vector3( 36, 0, 0 ) );
	collisionVertices.push_back( new Vector3( 40, -2, 0 ) );
	collisionVertices.push_back( new Vector3( 48, -8, 0 ) );
	collisionVertices.push_back( new Vector3( 52, -10, 0 ) );
	collisionVertices.push_back( new Vector3( 56, -13, 0 ) );
	collisionVertices.push_back( new Vector3( 54, -15, 0 ) );
	collisionVertices.push_back( new Vector3( 49, -14, 0 ) );
	collisionVertices.push_back( new Vector3( 38, -13, 0 ) );
	collisionVertices.push_back( new Vector3( 33, -12, 0 ) );
	collisionVertices.push_back( new Vector3( 23, -11, 0 ) );
	collisionVertices.push_back( new Vector3( 20, -10, 0 ) );
	collisionVertices.push_back( new Vector3( 15, -10, 0 ) );
	collisionVertices.push_back( new Vector3( 9, -15, 0 ) );
	collisionVertices.push_back( new Vector3( 6, -18, 0 ) );
	collisionVertices.push_back( new Vector3( 3, -20, 0 ) );

	collisionVertices.push_back( new Vector3( 0, -23, 0 ) ); // tail

	// Add the same vertices with negative x
	int count = collisionVertices.size();
	for ( int i = count - 2; i > 0; --i )
	{
		collisionVertices.push_back( new Vector3( -collisionVertices[i]->x, collisionVertices[i]->y, 0 ) );
	}
}

void PlayerShip::IncreaseVelocity( float amount )
{
	rigidbody->velocity = Vector3::ClampMagnitude( rigidbody->velocity + Forward() * amount, maxPlayerSpeed );
}

void PlayerShip::Draw( bool leftThruster, bool rightThruster ) const
{
	PreDraw();
	ActiveDraw( leftThruster, rightThruster );
	PostDraw();

	for ( auto &c : children )
	{
		c->Draw();
	}
}

void PlayerShip::FixedUpdate()
{
	// Slow the ship down over time
	float reduction = 1.5f;
	float magnitude = rigidbody->velocity.Magnitude();

	if ( magnitude > 0 )
	{
		rigidbody->velocity = Vector3::Normalize( rigidbody->velocity ) * max( 0, magnitude - reduction );
	}

	// Call parent's update (to move the rigidbody)
	GameObject::FixedUpdate();
}

void PlayerShip::ActiveDraw( bool leftThruster, bool rightThruster ) const
{
	// float colorArray1[4] = { 1, 1, 1, color.alpha }; glMaterialfv( GL_FRONT,
	// GL_AMBIENT_AND_DIFFUSE, colorArray1 );
	//
	// glPointSize( 4 ); glBegin( GL_POINTS ); for ( auto &v : collisionVertices ) { glVertex3f(
	// v->x, v->y, 3 ); } glEnd();

	float colorArray2[4] = { color.red, color.green, color.blue, color.alpha };
	glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colorArray2 );

	glRotatef( 90, 1, 0, 0 );
	glScalef( 25, 25, 25 );

	// Begin body
	glPushMatrix();
	glScalef( 1, .25, 1.5 ); // Y represents how "flat" the ship is, Z represents how elongated it is
	GLdouble eqn0[4] = { 0, 0, -1, 0 }; // Clip the rear half of the plane
	glClipPlane( GL_CLIP_PLANE0, eqn0 );
	glEnable( GL_CLIP_PLANE0 );
	glutSolidOctahedron();
	glDisable( GL_CLIP_PLANE0 );
	eqn0[2] = 1; // Clip the front half of the plane
	glClipPlane( GL_CLIP_PLANE0, eqn0 );
	glEnable( GL_CLIP_PLANE0 );
	glScalef( 1.0f, 1.0f, .6f ); // Draw the rear half shortened
	glutSolidOctahedron();
	glDisable( GL_CLIP_PLANE0 );
	glPopMatrix();

	// Begin right wing
	glPushMatrix();
	glRotatef( 90, 0, 1, 0 );
	glTranslatef( .25, 0, 0 ); // Push the wing towards the front of the plane so it doesn't look awkward when rotated backwards
	glRotatef( -20, 0, 1, 0 ); // Rotate the wing towards the rear
	glRotatef( 5, 1, 0, 0 ); // Tilt the wing down
	glScalef( 2, .25, 1 ); // X represents how wide the wings are (from ship nose to tail), Y represents how skinny they are (from ship top to bottom)
	glutSolidCone( .3, 2.5, 15, 15 );
	glPopMatrix();

	// Begin left wing
	glPushMatrix();
	glRotatef( -90, 0, 1, 0 );
	glTranslatef( -.25, 0, 0 );
	glRotatef( 20, 0, 1, 0 );
	glRotatef( 5, 1, 0, 0 );
	glScalef( 2, .25, 1 );
	glutSolidCone( .3, 2.5, 15, 15 );
	glPopMatrix();

	// Begin guns
	glPushMatrix();
	glTranslatef( .5f, -.15f, -1.25f ); // Y represents how close the guns are attached to the bottom of the ship, Z represents how close they are to the rear/front
	glScalef( 1, 1, .5 );
	glutSolidCylinder( .12, 3.75, 10, 5 );
	glTranslatef( -1.0, 0, 0 ); // X needs to be -2 times the prev x translate to be symmetrical
	glutSolidCylinder( .12, 3.75, 10, 5 );
	glPopMatrix();

	// Thrusters
	glPushMatrix();
	glTranslatef( .5f, -1, 0.5f );
	if ( leftThruster )
	{
		float color1[4] = { 1, Utility::Random( 0.75f, 1.0f ), Utility::Random( 0.0f, 1.0f ) ,Utility::Random( 0.5f, 0.75f ) };
		glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color1 );
		glutSolidCone( .12f, 0.75f, 10, 5 );
	}
	glTranslatef( -1.0, 0, 0 );
	if ( rightThruster )
	{
		float color2[4] = { 1, Utility::Random( 0.75f, 1.0f ), Utility::Random( 0.0f, 1.0f ),Utility::Random( 0.5f, 0.75f ) };
		glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color2 );
		glutSolidCone( .12f, 0.75f, 10, 5 );
	}
	glPopMatrix();
}

PlayerShip::~PlayerShip()
{
}