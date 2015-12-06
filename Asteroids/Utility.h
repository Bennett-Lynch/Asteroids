#pragma once

#define _USE_MATH_DEFINES

#include <random>
#include <cmath>

class Utility
{
public:
	static float Random( float minInclusive = 0, float maxInclusive = 1 );
	static int Random( int minInclusive = 1, int maxInclusive = 100 );
	static bool RandomBool();
	static float DegreesToRadians( float angle );
	static float RadiansToDegrees( float radians );
	static float Clamp( float value, float min, float max );
	static float Clamp01( float value );
	static int Clamp( int value, int min, int max );
	static float Lerp( float from, float to, float percent );
	static int PositiveMod( int num, int denom );
	static float PositiveMod( float num, float denom );

private:
	// Private constructor (to ensure that the Utility class cannot be instantiated)
	Utility() {}
};

struct Vector2
{
	// Public member variables
	float x, y;

	// Constructors
	Vector2( float x = 0, float y = 0 );

	// Unit vectors
	static Vector2 Down() { return Vector2( 0, -1 ); }
	static Vector2 Left() { return Vector2( -1, 0 ); }
	static Vector2 One() { return Vector2( 1, 1 ); }
	static Vector2 Right() { return Vector2( 1, 0 ); }
	static Vector2 Up() { return Vector2( 0, 1 ); }
	static Vector2 Zero() { return Vector2( 0, 0 ); }

	// Constant functions
	float Magnitude() const;

	// Mutating functions
	Vector2 Normalize();
	Vector2 Scale( float scalarModifier );
	Vector2 Translate( float inX = 0, float inY = 0 );
};

struct Vector3
{
	// Public member variables
	float x, y, z;

	// Constructors
	Vector3( float x = 0, float y = 0, float z = 0 );

	// Unit vectors
	static Vector3 Back() { return Vector3( 0, 0, -1 ); }
	static Vector3 Down() { return Vector3( 0, -1, 0 ); }
	static Vector3 Forward() { return Vector3( 0, 0, 1 ); }
	static Vector3 Left() { return Vector3( -1, 0, 0 ); }
	static Vector3 One() { return Vector3( 1, 1, 1 ); }
	static Vector3 Right() { return Vector3( 1, 0, 0 ); }
	static Vector3 Up() { return Vector3( 0, 1, 0 ); }
	static Vector3 Zero() { return Vector3( 0, 0, 0 ); }

	// Static functions
	static Vector3 Cross( Vector3 a, Vector3 b );
	//	static Vector3 Rotate( Vector3 primary, Vector3 rotation );
	static Vector3 ClampMagnitude( Vector3 vector, float maxMagnitude );
	static Vector3 Normalize( Vector3 vector );
	static float DistanceSquared( Vector3 a, Vector3 b ); // The squared distance between 2 vectors (note: this is much faster than using Distance)
	static float Distance( Vector3 a, Vector3 b ); // The distance between 2 vectors (note: this is much slower than using DistanceSquared)

	// Constant member functions
	float Magnitude() const;

	// Mutating functions
	Vector3 Normalize();
	Vector3 Scale( float scalarModifier );
	Vector3 Translate( float inX = 0, float inY = 0, float inZ = 0 );

	// Overloaded operators
	Vector3 operator-( const Vector3 &other ) const;
	Vector3 operator-=( const Vector3 &other );
	Vector3 operator+( const Vector3 &other ) const;
	Vector3 operator+=( const Vector3 &other );
	Vector3 operator*( float scalar ) const;
	Vector3 operator*=( float scalar );
};

struct Color
{
	// Public member variables
	float red, green, blue, alpha;

	// Constructors
	Color( float red = 0, float green = 0, float blue = 0, float alpha = 1 ); // For floats in range 0-1
	Color( int red, int green, int blue, int alpha = 255 ); // For ints in range 0-255

	// Standard colors
	static Color Red() { return Color( 1.0f, 0.0f, 0.0f ); }
	static Color Orange() { return Color( 1.0f, 0.5f, 0.0f ); }
	static Color Yellow() { return Color( 1.0f, 1.0f, 0.0f ); }
	static Color YellowGreen() { return Color( 0.5f, 1.0f, 0.0f ); }
	static Color Green() { return Color( 0.0f, 1.0f, 0.0f ); }
	static Color Turquoise() { return Color( 0.0f, 1.0f, 0.5f ); }
	static Color Cyan() { return Color( 0.0f, 1.0f, 1.0f ); }
	static Color Ocean() { return Color( 0.0f, 0.5f, 1.0f ); }
	static Color Blue() { return Color( 0.0f, 0.0f, 1.0f ); }
	static Color Violet() { return Color( 0.5f, 0.0f, 1.0f ); }
	static Color Magenta() { return Color( 1.0f, 0.0f, 1.0f ); }
	static Color Raspberry() { return Color( 1.0f, 0.0f, 0.5f ); }
	static Color White() { return Color( 1.0f, 1.0f, 1.0f ); }
	static Color Black() { return Color( 0.0f, 0.0f, 0.0f ); }
	static Color Grey() { return Color( 0.75f, 0.75f, 0.75f ); }

	// Static functions
	static Color Random() { return Color( Utility::Random( 0.0f, 1.0f ), Utility::Random( 0.0f, 1.0f ), Utility::Random( 0.0f, 1.0f ) ); }
};
