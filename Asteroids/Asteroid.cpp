#include "Asteroid.h"
#include "Rigidbody.h"

Asteroid::Asteroid( AsteroidSize size, Vector3 position, Vector3 rotation, Color color ) : GameObject( position, rotation, color )
{
	//	this->verticalVertices = Utility::Clamp( verticalVertices, 3, 50 );;
	//	this->horizontalVertices = Utility::Clamp( horizontalVertices, 3, 50 );;
	//	this->protrudeFactor = protrudeFactor;
	this->size = size;

	verticalVertices = horizontalVertices = 8;
	protrudeFactor = 0.50f;

	// Give the rigidbody component a random velocity
	rigidbody->velocity.x = Utility::Random( 20.0f, 200.0f ) * ( Utility::RandomBool() ? 1 : -1 );
	rigidbody->velocity.y = Utility::Random( 20.0f, 200.0f ) * ( Utility::RandomBool() ? 1 : -1 );

	// Randomly choose 1 of the 3 axes to rotate the object on; base the rotation speed on the velocity magnitude; and randomly make the angular velocity negative or positive
	switch ( Utility::Random( 1, 3 ) )
	{
	case 1:
		rigidbody->angularVelocity.x = rigidbody->velocity.Magnitude() * 0.5f * ( Utility::RandomBool() ? 1 : -1 );
		break;
	case 2:
		rigidbody->angularVelocity.y = rigidbody->velocity.Magnitude() * 0.5f * ( Utility::RandomBool() ? 1 : -1 );
		break;
	case 3:
		rigidbody->angularVelocity.z = rigidbody->velocity.Magnitude() * 0.5f * ( Utility::RandomBool() ? 1 : -1 );
		break;
	}

	// Controls the overall size of the asteroid
	float baseDiameter = 40;

	switch ( size )
	{
	case Large:
		baseDiameter *= 1.25f;
		break;
	case Medium:
		break;
	case Small:
		baseDiameter *= 0.75f;
		break;
	}

	float baseRadius = baseDiameter / 2;

	// Create a random height for the asteroid (otherwise it would always appear elongated with only x/z randomly increasing)
	float randomDiameter = baseDiameter * Utility::Random( 1.0f, 1 + protrudeFactor );
	radius = randomDiameter / 2;
	radiusSquared = pow( radius, 2 );

	// Create a 2-dimensional array for each vertex (where each vertex is represented as a 3D Vector)
	vertices = new Vector3*[horizontalVertices];
	for ( int i = 0; i < horizontalVertices; ++i )
	{
		vertices[i] = new Vector3[verticalVertices];
	}

	for ( int vert = 0; vert < verticalVertices; ++vert )
	{
		for ( int horiz = 0; horiz < horizontalVertices; ++horiz )
		{
			// This algorithm will result in the y slices being evenly distributed by a fixed amount
			float y = Utility::Lerp( radius, -radius, static_cast<float>( vert ) / ( verticalVertices - 1 ) );
			float phi = acosf( y / radius );
			float theta = Utility::Lerp( 0, static_cast<float>( 2 * M_PI ), static_cast<float>( horiz ) / horizontalVertices );

			// For every other row (odd rows), we want to rotate theta by half of one normal theta
			if ( vert % 2 == 1 )
			{
				theta += Utility::Lerp( 0, static_cast<float>( 2 * M_PI ), 0.5f / horizontalVertices );
			}

			// Calculate our x/y coords based on phi
			// Note that we use unit radius for these since we are going to achieve random scaling with each vertex
			float x = baseRadius * cosf( theta ) * sinf( phi );
			float z = baseRadius * sinf( theta ) * sinf( phi );

			// Make the horizontal vertices randomly protrude
			x *= Utility::Random( 1.0f, 1 + protrudeFactor );
			z *= Utility::Random( 1.0f, 1 + protrudeFactor );

			// Allow the vertical vertices to randomly change up to +/- portrude % of the heightstep
			// But only allow a random y portrusion to be set ONCE per top/bottom row (since they all converge to the same vertex)
			if ( ( vert == 0 || vert == verticalVertices - 1 ) && horiz != 0 )
			{
				y = vertices[0][vert].y;
			}
			else
			{
				float heightStep = randomDiameter / ( verticalVertices - 1 );
				y += Utility::Random( heightStep * ( 1 - protrudeFactor ), heightStep * ( 1 + protrudeFactor ) );
			}

			// Save the struct to our array or vertices
			vertices[horiz][vert].x = x;
			vertices[horiz][vert].z = z;
			vertices[horiz][vert].y = y;
		}
	}
}

void DrawTriangle( Vector3 a, Vector3 b, Vector3 c )
{
	glBegin( GL_POLYGON );

	GameObject::DefineNormal( a, b, c );

	glVertex3f( a.x, a.y, a.z );
	glVertex3f( b.x, b.y, b.z );
	glVertex3f( c.x, c.y, c.z );

	glEnd();
}

void Asteroid::ActiveDraw() const
{
	//	float colorArray1[4] = { 1, 1, 1, 1 };
	//	glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colorArray1 );
	//	glutSolidSphere( radius, 10, 10 );
	//	float colorArray2[4] = { color.red, color.green, color.blue, color.alpha };
	//	glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colorArray2 );

	Vector3 a, b, c;

	// We always start on the 2nd row (from the top) and draw "upwards" and counter-clockwise
	for ( int vert = 1; vert < verticalVertices; ++vert )
	{
		for ( int horiz = 0; horiz < horizontalVertices; ++horiz )
		{
			// Form a triangle in the shape of this -> up-right -> left -> this (see OneNote notes for image representation)
			if ( vert != 1 )
			{
				if ( vert % 2 == 1 ) // Odd
				{
					a = vertices[horiz][vert];
					b = vertices[Utility::PositiveMod( horiz + 1, horizontalVertices )][Utility::PositiveMod( vert - 1, verticalVertices )];
					c = vertices[horiz][Utility::PositiveMod( vert - 1, verticalVertices )];
				}
				else // Even
				{
					a = vertices[horiz][vert];
					b = vertices[horiz][Utility::PositiveMod( vert - 1, verticalVertices )];
					c = vertices[Utility::PositiveMod( horiz - 1, horizontalVertices )][Utility::PositiveMod( vert - 1, verticalVertices )];
				}

				DrawTriangle( a, b, c );
			}

			// Form a triangle in the shape of this -> right -> up -> this
			if ( vert != verticalVertices - 1 )
			{
				if ( vert % 2 == 1 ) // Odd
				{
					a = vertices[horiz][vert];
					b = vertices[Utility::PositiveMod( horiz + 1, horizontalVertices )][vert];
					c = vertices[Utility::PositiveMod( horiz + 1, horizontalVertices )][Utility::PositiveMod( vert - 1, verticalVertices )];
				}
				else // Even
				{
					a = vertices[horiz][vert];
					b = vertices[Utility::PositiveMod( horiz + 1, horizontalVertices )][vert];
					c = vertices[horiz][Utility::PositiveMod( vert - 1, verticalVertices )];
				}

				DrawTriangle( a, b, c );
			}
		}
	}
}

Asteroid::~Asteroid()
{
	for ( int horiz = 0; horiz < horizontalVertices; ++horiz )
	{
		delete[] vertices[horiz];
	}
	delete[] vertices;
}