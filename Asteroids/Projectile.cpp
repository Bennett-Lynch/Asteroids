#include "Projectile.h"
#include "Globals.h"

Projectile::Projectile( Vector3 position, Vector3 rotation, Color color ) : GameObject( position, rotation, color )
{
	radius = 5;
	radiusSquared = pow( radius, 2 );
}

void Projectile::ActiveDraw() const
{
	glutSolidSphere( 5, 10, 10 );

	//	glPointSize( 10 );
	//	glBegin( GL_POINTS );
	//	glVertex3f( 0, 0, 0 );
	//	glEnd();
}

Projectile::~Projectile()
{
}