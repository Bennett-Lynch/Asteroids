/*
Citations:
Some boilerplate OpenGL/GLUT code partially borrowed from: http://www.linuxfocus.org/English/January1998/article17.html
*/

#include <chrono>
#include "Utility.h"
#include "Asteroid.h"
#include "PlayerShip.h"
#include "AlienShip.h"
#include "Projectile.h"
#include "Explosion.h"
#include <string>
#include <math.h>

// Define externs
Vector2 screenResolution = { 1280, 720 };
float updateFPS = 60;
float fixedDeltaTime = 1 / updateFPS;
float viewPaneZoom = 1;
float visibleX = screenResolution.x / viewPaneZoom;
float visibleY = screenResolution.y / viewPaneZoom;
float halfVisibleX = visibleX / 2;
float halfVisibleY = visibleY / 2;
Color backgroundColor = Color( 0.15f, 0.15f, 0.15f );
float screenBorder = 20;
float playerAcceleration = 5;
float maxPlayerSpeed = 500;
float playerRotationSpeed = 3;
float baseProjectileSpeed = 550;
float projectileSpawnOffset = 40;
unsigned int maxProjectiles = 6;

// Global typedefs
typedef std::chrono::high_resolution_clock Time;
typedef Time::time_point Timestamp;
typedef std::chrono::milliseconds ms;
typedef std::chrono::duration<float> fsec;

// Global enums
enum MENU_OPTION { NEW_GAME, MENU_QUIT };

// Booleans to track whether certain keys are currently being held down or not
bool isADown, isDDown, isLeftDown, isRightDown, isUpDown, isWDown, isXDown;

// Misc
const Timestamp startTime = Time::now();
bool gameActive = false;
float startTextTime;
std::string hudText = "";
float lastTimePointChange = -1;
float playerSpawnTime;
const float playerImmuneTime = 2;

// Game object variables
PlayerShip* playerShip = nullptr;
std::vector<Projectile*> playerProjectiles;
int playerScore = 0;
int startingLives = 2;
int respawnsRemaining = startingLives;
int bonusShipMultiple = 10000;
int bonusLiveCounter = 0;

// Asteroid variables
const unsigned int maxNumAsteroids = 20; // Project req's suggested 6, but increasing to make sure game isn't infinitely beatable (since 6 is fairly easy to survive)
unsigned int startingNumAsteroids = 3;
unsigned int numAsteroids = startingNumAsteroids;
std::vector<Asteroid*> asteroids;

// Alien variables
const float minTimeBetweenAlienSpawn = 10;
float lastAlienDespawnTime = -minTimeBetweenAlienSpawn;
const int minScoreForSmallShip = 5000;
AlienShip* alienShip = nullptr;
Projectile* alienProjectile = nullptr;

// Explosion variables
std::vector<Explosion*> explosions;

// Point values
const int largeAsteroidPoints = 20;
const int mediumAsteroidPoints = 50;
const int smallAsteroidPoints = 100;
const int largeAlienPoints = 200;
const int smallAlienPoints = 1000;

// Function prototypes
void SetupProjection();
void SetupCamera();
void EnableLighting();
void EnableBlending();
void SetupMenu();
void CreateLargeAsteroids( int quantity );
void CreateAlienShip();
void DisplayCallback();
void IdleCallback();
void StartNewGame();
void DrawInfoText();
void NewHUDText( std::string text );
void DrawHUDText();
void UpdateCallback( int value );
void CheckPlayerProjectileAsteroidCollisions();
void CheckPlayerAsteroidCollisions();
void CheckPlayerProjectileAlienCollisions();
void CheckPlayerAlienCollisions();
void CheckAlienProjectilePlayerCollisions();
void CheckAlienProjectileAsteroidCollisions();
void CheckAlienAsteroidCollisions();
void DestroyAsteroid( int asteroidIndex, bool awardPoints );
void DestroyProjectile( int projectileIndex );
void DestroyPlayer();
void DestroyAlien( bool explode, bool awardPoints );
void DestroyAlienProjectile();
void AwardPoints( int points );
void MenuCallback( int value );
void KeyPressCallback( unsigned char key, int mouseX, int mouseY );
void KeyReleaseCallback( unsigned char key, int mouseX, int mouseY );
void SpecialKeyPressCallback( int key, int mouseX, int mouseY );
void SpecialKeyReleaseCallback( int key, int mouseX, int mouseY );
void PlayerShoot();
void AlienShoot();
float ProgramTime();
void DeleteAllObjects();

int main( int argcp, char *argv[] )
{
	// Initialize GLUT state
	glutInit( &argcp, argv );
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
	glutInitWindowSize( static_cast<int>( screenResolution.x ), static_cast<int>( screenResolution.y ) );
	glutInitWindowPosition( static_cast<int>( ( glutGet( GLUT_SCREEN_WIDTH ) - screenResolution.x ) / 2 ), static_cast<int>( ( glutGet( GLUT_SCREEN_HEIGHT ) - screenResolution.y ) / 2 ) );

	// Create a window
	glutCreateWindow( "Asteroids" );

	// Configure OpenGL settings to setup our camera
	SetupProjection();
	SetupCamera();

	// Enable lighting for the scene
	EnableLighting();

	// Enable blending
	EnableBlending();

	// Register display callback functions
	glutDisplayFunc( DisplayCallback );
	glutIdleFunc( IdleCallback );

	// Register keyboard callback functions
	glutKeyboardFunc( KeyPressCallback );
	glutKeyboardUpFunc( KeyReleaseCallback );
	glutSpecialFunc( SpecialKeyPressCallback );
	glutSpecialUpFunc( SpecialKeyReleaseCallback );

	// Register our self-looping update function
	glutTimerFunc( static_cast<int>( 1000 / updateFPS ), UpdateCallback, 0 );

	// Create a menu
	SetupMenu();

	// Register our manual memory cleanup function
	atexit( DeleteAllObjects );

	NewHUDText( "Right click to start a new game!" );

	// Start the main game loop
	glutMainLoop();

	return 0;
};

void SetupProjection()
{
	// Setup projection
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	// Setup the clipping plane
	glOrtho( -screenResolution.x / viewPaneZoom / 2, screenResolution.x / viewPaneZoom / 2, -screenResolution.y / viewPaneZoom / 2, screenResolution.y / viewPaneZoom / 2, -1000, 1000 );

	// Enable backface culling (to improve performance)
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
}

void SetupCamera()
{
	// Setup model transformations
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	// Position and orient the camera to face the origin (eye, center, up)
	gluLookAt( 0, 0, 5, 0, 0, 0, 0, 1, 0 );
}

void EnableLighting()
{
	//  Set default ambient light in the scene
	float globalAmbient[4] = { .25, .25, .25, 1 }; // global ambient light intensity, default is (.2, .2, .2, 1)
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, globalAmbient );

	float position[4] = { -50.0f, 50.0f, 1.0f, 0.0f }; // position of light, default is( 0, 0, 1, 1 ), if w=0 it is assumed to be at infinity
	float ambient[4] = { .6f, .6f, .6f, 1.0f }; //  intensity of ambient contribution, default is (0, 0, 0, 0)
	float diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // intensity of diffuse contribution, default is (1, 1, 1, 1)
	float specular[4] = { .2f, .2f, .2f, 1.0f }; //  intensity of specular contribution, default is (1, 1, 1, 1)

	//  Set Light 0 position, ambient, diffuse, specular intensities
	glLightfv( GL_LIGHT0, GL_POSITION, position );
	glLightfv( GL_LIGHT0, GL_AMBIENT, ambient );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
	glLightfv( GL_LIGHT0, GL_SPECULAR, specular );

	//  Enable Light 0 and then GL lighting as a whole
	glEnable( GL_LIGHT0 );
	glEnable( GL_LIGHTING );

	glShadeModel( GL_FLAT ); // Flat shading (as opposed to smooth)
	glEnable( GL_NORMALIZE ); // Normalize normals

	// Specify the clear value for the depth buffer
	glClearDepth( 1.0 );
	glEnable( GL_DEPTH_TEST );
}

void EnableBlending()
{
	// Enable blending so particles can slowly fade out
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_BLEND );

	// Setup the background color to be used on every glClear call
	glClearColor( backgroundColor.red, backgroundColor.green, backgroundColor.blue, backgroundColor.alpha );

	// Make points circular rather than round
	glEnable( GL_POINT_SMOOTH );
}

void SetupMenu()
{
	glutCreateMenu( MenuCallback );

	// Add a menu items for each menu enum
	glutAddMenuEntry( "New Game", NEW_GAME );
	glutAddMenuEntry( "Quit", MENU_QUIT );

	// Associate a mouse button with menu
	glutAttachMenu( GLUT_RIGHT_BUTTON );
}

// Initiate a new level by creating x amount of large asteroids
void CreateLargeAsteroids( int quantity )
{
	for ( int i = 0; i < quantity; ++i )
	{
		Vector3 randomPos = Vector3();

		// Pick a random side of the screen to spawn on
		switch ( Utility::Random( 1, 4 ) )
		{
		case 1:
			randomPos.x = -( halfVisibleX + screenBorder );
			randomPos.y = Utility::Random( -halfVisibleY, halfVisibleY );
			break;
		case 2:
			randomPos.x = halfVisibleX + screenBorder;
			randomPos.y = Utility::Random( -halfVisibleY, halfVisibleY );
			break;
		case 3:
			randomPos.x = Utility::Random( -halfVisibleX, halfVisibleX );
			randomPos.y = -( halfVisibleY + screenBorder );
			break;
		case 4:
			randomPos.x = Utility::Random( -halfVisibleX, halfVisibleX );
			randomPos.y = halfVisibleY + screenBorder;
			break;
		}

		asteroids.push_back( new Asteroid( Asteroid::Large, randomPos ) );
	}

	// Reset the alien despawn timer
	lastAlienDespawnTime = -minTimeBetweenAlienSpawn;
}

void CreateAlienShip()
{
	// If last despawn time is positive that means there has already been at least 1 alien ship on this level (since it is reset to negative on every new level)
	if ( lastAlienDespawnTime > 0 && playerScore >= minScoreForSmallShip )
	{
		alienShip = new AlienShip( AlienShip::Small );
	}
	else
	{
		alienShip = new AlienShip( AlienShip::Large );
	}

	// Pick a random side of the screen to spawn on
	switch ( Utility::Random( 1, 2 ) )
	{
	case 1:
		alienShip->position.x = -( halfVisibleX + screenBorder );
		alienShip->position.y = Utility::Random( -halfVisibleY, halfVisibleY );

		alienShip->rigidbody->velocity.x = Utility::Random( 30.0f, 50.0f );
		break;
	case 2:
		alienShip->position.x = halfVisibleX + screenBorder;
		alienShip->position.y = Utility::Random( -halfVisibleY, halfVisibleY );

		alienShip->rigidbody->velocity.x = Utility::Random( -30.0f, -50.0f );
		break;
	}
}

// Display function (called every frame)
void DisplayCallback()
{
	// Draw the background
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	if ( playerShip != nullptr )
	{
		bool forward = isWDown || isXDown || isUpDown;
		int rotationInput = min( 1, isDDown + isRightDown ) - min( 1, isADown + isLeftDown );
		playerShip->Draw( forward && rotationInput <= 0, forward && rotationInput >= 0 );
	}

	if ( alienShip != nullptr )
	{
		alienShip->Draw();
	}

	if ( alienProjectile != nullptr )
	{
		alienProjectile->Draw();
	}

	for ( auto &a : asteroids )
	{
		a->Draw();
	}

	for ( auto &p : playerProjectiles )
	{
		p->Draw();
	}

	for ( auto &e : explosions )
	{
		e->Draw();
	}

	glDisable( GL_LIGHTING );
	DrawInfoText();
	DrawHUDText();
	glEnable( GL_LIGHTING );

	// After drawing the frame we swap the buffers (so that the user cannot see the scene being assembled)
	glutSwapBuffers();
};

void DrawInfoText()
{
	float elapsed = ProgramTime() - lastTimePointChange;
	glColor4f( 1.0f, 1.0f, elapsed < 1 ? elapsed : 1.0f, 1.0f );
	glRasterPos2i( static_cast<GLint>( halfVisibleX - 125 ), static_cast<GLint>( halfVisibleY - 30 ) );

	// Score
	glutBitmapString( GLUT_BITMAP_HELVETICA_18, reinterpret_cast<unsigned char const*>( "Score: " ) );

	std::string scoreText = std::to_string( playerScore );
	glutBitmapString( GLUT_BITMAP_HELVETICA_18, reinterpret_cast<unsigned char const*>( scoreText.c_str() ) );

	glRasterPos2i( static_cast<GLint>( halfVisibleX - 125 ), static_cast<GLint>( halfVisibleY - 50 ) );

	// Respawns
	glutBitmapString( GLUT_BITMAP_HELVETICA_18, reinterpret_cast<unsigned char const*>( "Respawns: " ) );

	std::string respawnText = std::to_string( respawnsRemaining );
	glutBitmapString( GLUT_BITMAP_HELVETICA_18, reinterpret_cast<unsigned char const*>( respawnText.c_str() ) );
}

void NewHUDText( std::string text )
{
	hudText = text;
	startTextTime = ProgramTime();
}

void DrawHUDText()
{
	float elapsed = ProgramTime() - startTextTime;
	if ( elapsed < 5 )
	{
		glColor4f( 1.0f, 1.0f, 1.0f, elapsed < 2 ? 1.0f : 1 - ( elapsed - 2 ) / 3 );

		int stringSize = glutBitmapLength( GLUT_BITMAP_HELVETICA_18, reinterpret_cast<unsigned char const*>( hudText.c_str() ) );
		glRasterPos2i( static_cast<GLint>( -stringSize / 2 ), static_cast<GLint>( halfVisibleY - 150 ) );

		glutBitmapString( GLUT_BITMAP_HELVETICA_18, reinterpret_cast<unsigned char const*>( hudText.c_str() ) );
	}
}

// Called when there was no interaction from the player during the previous frame
void IdleCallback()
{
	// Redraw the display
	glutPostRedisplay();
};

void StartNewGame()
{
	DeleteAllObjects();

	playerShip = new PlayerShip();
	//	playerSpawnTime = ProgramTime(); // Commenting out to just make player immune on dying

	playerScore = bonusLiveCounter = 0;
	respawnsRemaining = startingLives;
	numAsteroids = startingNumAsteroids;
	gameActive = true; // Setting gameActive to true will then spawn asteroids in the update callback
}

void UpdateCallback( int value )
{
	if ( playerShip != nullptr )
	{
		// Sum up all of our input axes (so they can cancel each other out)
		int rotationInput = min( 1, isDDown + isRightDown ) - min( 1, isADown + isLeftDown );
		playerShip->Rotate( 0, 0, -rotationInput * playerRotationSpeed );

		if ( isWDown || isXDown || isUpDown )
		{
			playerShip->IncreaseVelocity( playerAcceleration );
		}

		// Update player ship
		playerShip->FixedUpdate();
	}

	// If there are no asteroids in the scene, spawn a new set of large ones
	if ( gameActive && asteroids.empty() )
	{
		CreateLargeAsteroids( numAsteroids );
		numAsteroids = min( numAsteroids + 1, maxNumAsteroids );
	}

	// Update asteroids
	for ( auto &a : asteroids )
	{
		a->FixedUpdate();
	}

	// If we don't currently have an alien ship, see if conditions are right to spawn one...
	if ( alienShip == nullptr && !asteroids.empty() && ProgramTime() - lastAlienDespawnTime >= minTimeBetweenAlienSpawn )
	{
		// Get a sum of num asteroids remaining
		int totalAsteroids = 0;
		for ( auto &a : asteroids )
		{
			switch ( a->size )
			{
			case Asteroid::Large:
				totalAsteroids += 7;
				break;
			case Asteroid::Medium:
				totalAsteroids += 3;
				break;
			case Asteroid::Small:
				totalAsteroids += 1;
				break;
			}
		}

		// If less than half of the starting asteroids remain, spawn an alien ship
		if ( totalAsteroids <= numAsteroids * 7 * 0.5f )
		{
			CreateAlienShip();
		}
	}

	// Update alien ship
	if ( alienShip != nullptr )
	{
		alienShip->FixedUpdate(); // AlienShip's override of FixedUpdate will handle randomly changing its y-velocity

								  // If the alien ship has traveled off the screen (horizontally), destroy it
		if ( alienShip->position.x < -( halfVisibleX + screenBorder ) || alienShip->position.x >  halfVisibleX + screenBorder )
		{
			DestroyAlien( false, false );
		}
		// If the alien ship has no existing projectile, fire a new one
		else if ( alienProjectile == nullptr )
		{
			AlienShoot();
		}
	}

	// Update alien projectile
	if ( alienProjectile != nullptr )
	{
		if ( ProgramTime() - alienProjectile->spawnTime > alienProjectile->lifeTime )
		{
			DestroyAlienProjectile();
		}
		else
		{
			alienProjectile->FixedUpdate();
		}
	}

	// Update playerProjectiles
	for ( size_t i = 0; i < playerProjectiles.size(); ++i )
	{
		if ( ProgramTime() - playerProjectiles[i]->spawnTime > playerProjectiles[i]->lifeTime )
		{
			delete playerProjectiles[i];
			playerProjectiles.erase( playerProjectiles.begin() + i );
			i--;
		}
		else
		{
			playerProjectiles[i]->FixedUpdate();
		}
	}

	// Handle collisions (which may spawn explosions) (nullptr checking is handled inside each function)
	CheckPlayerProjectileAsteroidCollisions();
	CheckPlayerAsteroidCollisions();
	CheckPlayerProjectileAlienCollisions();
	CheckPlayerAlienCollisions();
	CheckAlienProjectilePlayerCollisions();
	CheckAlienProjectileAsteroidCollisions();
	CheckAlienAsteroidCollisions();

	// Update explosions
	for ( size_t i = 0; i < explosions.size(); ++i )
	{
		if ( ProgramTime() - explosions[i]->spawnTime > explosions[i]->lifeTime )
		{
			delete explosions[i];
			explosions.erase( explosions.begin() + i );
			i--;
		}
		else
		{
			explosions[i]->FixedUpdate();
		}
	}

	// Schedule a reloop
	glutTimerFunc( static_cast<int>( 1000 / updateFPS ), UpdateCallback, 0 );
}

void CheckPlayerProjectileAsteroidCollisions()
{
	for ( size_t p = 0; p < playerProjectiles.size(); ++p )
	{
		for ( size_t a = 0; a < asteroids.size(); ++a )
		{
			if ( Vector3::DistanceSquared( playerProjectiles[p]->position, asteroids[a]->position ) <= asteroids[a]->radiusSquared )
			{
				DestroyAsteroid( a, true );
				DestroyProjectile( p );
				return;
			}
		}
	}
}

void CheckPlayerAsteroidCollisions()
{
	if ( playerShip == nullptr || ProgramTime() - playerSpawnTime < playerImmuneTime ) return;

	for ( size_t a = 0; a < asteroids.size(); ++a )
	{
		for ( auto &v : playerShip->collisionVertices )
		{
			// Rotate the collision vertices by the player rotation and see if they're inside of the asteroid
			if ( Vector3::DistanceSquared( playerShip->position + Vector3( v->x * cosf( Utility::DegreesToRadians( playerShip->rotation.z ) ) - v->y * sinf( Utility::DegreesToRadians( playerShip->rotation.z ) ), v->x * sinf( Utility::DegreesToRadians( playerShip->rotation.z ) ) + v->y * cosf( Utility::DegreesToRadians( playerShip->rotation.z ) ), 0 ), asteroids[a]->position ) <= asteroids[a]->radiusSquared )
			{
				DestroyAsteroid( a, true );
				DestroyPlayer();
				return;
			}
		}
	}
}

void CheckPlayerProjectileAlienCollisions()
{
	if ( alienShip == nullptr ) return;

	for ( size_t p = 0; p < playerProjectiles.size(); ++p )
	{
		if ( Vector3::DistanceSquared( playerProjectiles[p]->position, alienShip->position ) <= alienShip->radiusSquared )
		{
			DestroyAlien( true, true );
			DestroyProjectile( p );
			return;
		}
	}
}

void CheckPlayerAlienCollisions()
{
	if ( playerShip == nullptr || ProgramTime() - playerSpawnTime < playerImmuneTime || alienShip == nullptr ) return;

	for ( auto &v : playerShip->collisionVertices )
	{
		// Rotate the collision vertices by the player rotation and see if they're inside of the asteroid
		if ( Vector3::DistanceSquared( playerShip->position + Vector3( v->x * cosf( Utility::DegreesToRadians( playerShip->rotation.z ) ) - v->y * sinf( Utility::DegreesToRadians( playerShip->rotation.z ) ), v->x * sinf( Utility::DegreesToRadians( playerShip->rotation.z ) ) + v->y * cosf( Utility::DegreesToRadians( playerShip->rotation.z ) ), 0 ), alienShip->position ) <= alienShip->radiusSquared )
		{
			DestroyAlien( true, true );
			DestroyPlayer();
			return;
		}
	}
}

void CheckAlienProjectilePlayerCollisions()
{
	if ( playerShip == nullptr || ProgramTime() - playerSpawnTime < playerImmuneTime || alienProjectile == nullptr ) return;

	for ( auto &v : playerShip->collisionVertices )
	{
		// Rotate the collision vertices by the player rotation and see if they're inside of the asteroid
		if ( Vector3::DistanceSquared( playerShip->position + Vector3( v->x * cosf( Utility::DegreesToRadians( playerShip->rotation.z ) ) - v->y * sinf( Utility::DegreesToRadians( playerShip->rotation.z ) ), v->x * sinf( Utility::DegreesToRadians( playerShip->rotation.z ) ) + v->y * cosf( Utility::DegreesToRadians( playerShip->rotation.z ) ), 0 ), alienProjectile->position ) <= alienProjectile->radiusSquared )
		{
			DestroyAlienProjectile();
			DestroyPlayer();
			return;
		}
	}
}

void CheckAlienProjectileAsteroidCollisions()
{
	if ( alienProjectile == nullptr ) return;

	for ( size_t a = 0; a < asteroids.size(); ++a )
	{
		if ( Vector3::DistanceSquared( alienProjectile->position, asteroids[a]->position ) <= asteroids[a]->radiusSquared )
		{
			DestroyAsteroid( a, false );
			DestroyAlienProjectile();
			return;
		}
	}
}

void CheckAlienAsteroidCollisions()
{
	if ( alienShip == nullptr ) return;

	for ( size_t a = 0; a < asteroids.size(); ++a )
	{
		if ( Vector3::DistanceSquared( alienShip->position, asteroids[a]->position ) <= alienShip->radiusSquared + asteroids[a]->radiusSquared )
		{
			DestroyAlien( true, false );
			DestroyAsteroid( a, false );
			return;
		}
	}
}

void DestroyAsteroid( int index, bool awardPoints )
{
	switch ( asteroids[index]->size )
	{
	case Asteroid::Large:
		explosions.push_back( new Explosion( 100, 1.25f, asteroids[index]->position ) );

		asteroids.push_back( new Asteroid( Asteroid::Medium, asteroids[index]->position ) );
		asteroids.push_back( new Asteroid( Asteroid::Medium, asteroids[index]->position ) );

		if ( awardPoints )
		{
			AwardPoints( largeAsteroidPoints );
		}
		break;
	case Asteroid::Medium:
		explosions.push_back( new Explosion( 100, 1.00f, asteroids[index]->position ) );

		asteroids.push_back( new Asteroid( Asteroid::Small, asteroids[index]->position ) );
		asteroids.push_back( new Asteroid( Asteroid::Small, asteroids[index]->position ) );

		if ( awardPoints )
		{
			AwardPoints( mediumAsteroidPoints );
		}
		break;
	case Asteroid::Small:
		explosions.push_back( new Explosion( 100, 0.75f, asteroids[index]->position ) );

		if ( awardPoints )
		{
			AwardPoints( smallAsteroidPoints );
		}
		break;
	}

	// Remove the asteroid
	delete asteroids[index];
	asteroids.erase( asteroids.begin() + index );
}

void DestroyProjectile( int index )
{
	// Remove the projectile
	delete playerProjectiles[index];
	playerProjectiles.erase( playerProjectiles.begin() + index );
}

void DestroyPlayer()
{
	// Explode and respawn the player ship
	explosions.push_back( new Explosion( 100, 1.25f, playerShip->position ) );

	delete playerShip;

	if ( respawnsRemaining > 0 )
	{
		respawnsRemaining--;
		playerShip = new PlayerShip();
		playerSpawnTime = ProgramTime();
	}
	else
	{
		playerShip = nullptr;
		gameActive = false;
		NewHUDText( "Game Over!" );
	}

	// Update the timestamp so we can flash the info text yellow
	lastTimePointChange = ProgramTime();
}

void DestroyAlien( bool explode, bool awardPoints )
{
	if ( explode )
	{
		explosions.push_back( new Explosion( 100, alienShip->size == AlienShip::Large ? 1.50f : 1.25f, alienShip->position ) );
	}

	if ( awardPoints )
	{
		AwardPoints( alienShip->size == AlienShip::Large ? largeAlienPoints : smallAlienPoints );
	}

	delete alienShip;
	alienShip = nullptr;
	lastAlienDespawnTime = ProgramTime();
}

void DestroyAlienProjectile()
{
	delete alienProjectile;
	alienProjectile = nullptr;
}

void AwardPoints( int points )
{
	if ( !gameActive ) return;

	playerScore += points;

	bonusLiveCounter += points;
	while ( bonusLiveCounter >= bonusShipMultiple )
	{
		respawnsRemaining++;
		bonusLiveCounter -= bonusShipMultiple;
		NewHUDText( "Additional respawn earned for earning 10k points!" );
	}

	// Update the timestamp so we can flash the info text yellow
	lastTimePointChange = ProgramTime();
}

void MenuCallback( int value )
{
	switch ( value )
	{
	case NEW_GAME:
		StartNewGame();
		break;
	case MENU_QUIT:
		exit( 0 );
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

void KeyPressCallback( unsigned char key, int mouseX, int mouseY )
{
	switch ( toupper( key ) )
	{
	case 'A':
		isADown = true;
		break;
	case 'D':
		isDDown = true;
		break;
	case 'W':
		isWDown = true;
		break;
	case 'X':
		isXDown = true;
		break;
	case 'Z':
	case 'S':
	case ' ':
		PlayerShoot();
		break;
	}
}

void KeyReleaseCallback( unsigned char key, int mouseX, int mouseY )
{
	switch ( toupper( key ) )
	{
	case 'A':
		isADown = false;
		break;
	case 'D':
		isDDown = false;
		break;
	case 'W':
		isWDown = false;
		break;
	case 'X':
		isXDown = false;
		break;
	}
}

void SpecialKeyPressCallback( int key, int mouseX, int mouseY )
{
	switch ( key )
	{
	case GLUT_KEY_LEFT:
		isLeftDown = true;
		break;
	case GLUT_KEY_RIGHT:
		isRightDown = true;
		break;
	case GLUT_KEY_UP:
		isUpDown = true;
		break;
	}
}

void SpecialKeyReleaseCallback( int key, int mouseX, int mouseY )
{
	switch ( key )
	{
	case GLUT_KEY_LEFT:
		isLeftDown = false;
		break;
	case GLUT_KEY_RIGHT:
		isRightDown = false;
		break;
	case GLUT_KEY_UP:
		isUpDown = false;
		break;
	}
}

void PlayerShoot()
{
	if ( playerShip == nullptr ) return;

	// Spawn the projectile at the location of the player ship, offset some units in front of it
	Projectile* p = new Projectile( playerShip->position + playerShip->Forward() * projectileSpawnOffset );

	// Set the projectile's initial direction equal to the facing direction of the ship + its current velocity
	p->rigidbody->velocity = playerShip->Forward() * baseProjectileSpeed + playerShip->rigidbody->velocity;

	if ( playerProjectiles.size() >= maxProjectiles )
	{
		DestroyProjectile( 0 );
	}

	playerProjectiles.push_back( p );
}

// As the alien ship moves, it will shoot single shots at either an asteroid, the player's ship, or in a random direction.
void AlienShoot()
{
	// Spawn the projectile at the location of the alien ship
	Projectile* p = new Projectile( alienShip->position );
	p->color = alienShip->color;

	Vector3 projectileDirection;

	/*
	After an alien ship appears, it shoots single shots repeatedly until it moves offscreen. An alien ship aims its shot in three ways:
	(1) at an asteroid with probability pa, (2) at the player's ship with probability ps, or (3) in a random direction with probability pr.
	The probabilities will be different for the different types of alien ships, for example:
	large alien, pa = 0.3, ps = 0.2, pr = 0.5.
	small alien, pa = 0.4, ps = 0.5, pr = 0.1.
	*/

	float randomRoll = Utility::Random( 0.0f, 1.0f );

	bool large = alienShip->size == AlienShip::Large;

	if ( !asteroids.empty() && randomRoll <= ( large ? 0.3f : 0.4f ) )
	{
		// Choose a random asteroid to shoot at
		projectileDirection = asteroids[Utility::Random( 0, asteroids.size() - 1 )]->position - alienShip->position;
	}
	else if ( playerShip != nullptr && randomRoll <= ( large ? 0.5f : 0.9f ) )
	{
		// Shoot at the player
		projectileDirection = playerShip->position - alienShip->position;
	}
	else
	{
		// Choose a random direction (in radians)
		float randomAngle = Utility::Random( 0.0f, static_cast<float>( 2 * M_PI ) );
		projectileDirection = Vector3( cosf( randomAngle ), sinf( randomAngle ), 0 );
	}

	// Set the projectile's initial direction equal to the facing direction of the ship + its current velocity
	p->rigidbody->velocity = projectileDirection.Normalize() * baseProjectileSpeed + alienShip->rigidbody->velocity;

	alienProjectile = p;
}

float ProgramTime()
{
	return static_cast<fsec>( Time::now() - startTime ).count();

	//	auto t0 = Time::now();
	//	auto t1 = Time::now();
	//	fsec fs = Time::now() - startTime;
	//	ms d = std::chrono::duration_cast<ms>( fs );
	//	std::cout << fs.count() << "s\n";
	//	std::cout << d.count() << "ms\n";
}

void DeleteAllObjects()
{
	if ( playerShip != nullptr )
	{
		delete playerShip;
		playerShip = nullptr;
	}

	if ( alienShip != nullptr )
	{
		delete alienShip;
		alienShip = nullptr;
	}

	for ( auto &a : asteroids )
	{
		delete a;
	}
	asteroids.clear();

	for ( auto &p : playerProjectiles )
	{
		delete p;
	}
	playerProjectiles.clear();

	if ( alienProjectile != nullptr )
	{
		delete alienProjectile;
		alienProjectile = nullptr;
	}

	for ( auto &e : explosions )
	{
		delete e;
	}
	explosions.clear();
}