#include "Explosion.h"
#include "Particle.h"
#include <iostream>
#include "Globals.h"

Explosion::Explosion( int numParticles, float particleScalar, Vector3 position, Vector3 rotation, Color color ) : GameObject( position, rotation, color )
{
	for ( int i = 0; i < numParticles; ++i )
	{
		Particle* p = new Particle( Utility::Random( 0.75f, 1.25f ) * particleScalar, position );

		// Choose a random circular emitting direction
		float randomAngle = Utility::Random( 0.0f, 360.0f );
		float randomMagnitude = Utility::Random( 0.0f, 100.0f ) * particleScalar;
		p->rigidbody->velocity = Vector3( randomMagnitude * cosf( randomAngle ), randomMagnitude * sinf( randomAngle ), 0 );

		children.push_back( p );
	}

	float pos[4] = { position.x, position.y, position.z, 1 }; // position of light, default is( 0, 0, 1, 1 ), if w=0 it is assumed to be at infinity
	//  Set Light 0 position, ambient, diffuse, specular intensities
	glLightfv( GL_LIGHT1, GL_POSITION, pos );
}

void Explosion::FixedUpdate()
{
	GameObject::FixedUpdate();

	float percent = Utility::Lerp( 1, 0, ( ProgramTime() - spawnTime ) / lifeTime );

	// Fade out the transparency of all of the particles
	for ( auto& c : children )
	{
		//		c->color.alpha = Utility::Lerp( 1, 0, ( ProgramTime() - spawnTime ) / lifeTime );
		c->color.alpha = percent;
	}

	//	Color color = Color( 255, Utility::Random( 128, 255 ), 0 )

	// TODO support muultiple explosion light sources
	float ambient[4] = { 0.15f * percent, 0.125f * percent, 0, 1 }; //  intensity of ambient contribution, default is (0, 0, 0, 0)
	float diffuse[4] = { 0 * percent, 0 * percent, 0, 0 * percent }; // intensity of diffuse contribution, default is (1, 1, 1, 1)
	float specular[4] = { 0 * percent, 0 * percent, 0, 1 }; //  intensity of specular contribution, default is (1, 1, 1, 1)

	// Set Light 0 position, ambient, diffuse, specular intensities
	glLightfv( GL_LIGHT1, GL_AMBIENT, ambient );
	glLightfv( GL_LIGHT1, GL_DIFFUSE, diffuse );
	glLightfv( GL_LIGHT1, GL_SPECULAR, specular );

	//  Enable Light 0 and then GL lighting as a whole
	glEnable( GL_LIGHT1 );
}