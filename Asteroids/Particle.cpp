#include "Particle.h"
#include <iostream>

Particle::Particle( float scale, Vector3 position, Vector3 rotation, Color color ) : GameObject( position, rotation, color )
{
	this->scale = scale;
}

void Particle::ActiveDraw() const
{
	glutSolidSphere( scale, 5, 5 );

	//	glPointSize( 2 );
	//	glBegin( GL_POINTS );
	//	glVertex3f( 0, 0, 0 );
	//	glEnd();
}