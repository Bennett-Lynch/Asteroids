/*
Citations:
"Random" code partially borrowed from: http://stackoverflow.com/questions/19665818/best-way-to-generate-random-numbers-using-c11-random-library
Many struct and function concepts (but not code) borrowed from Unity3D
*/

#include "Utility.h"

float Utility::Random( float minInclusive, float maxInclusive )
{
	if ( minInclusive > maxInclusive )
	{
		float temp = minInclusive;
		minInclusive = maxInclusive;
		maxInclusive = temp;
	}

	std::random_device randDevice;
	std::mt19937 randNumGenerator( randDevice() );
	std::uniform_real_distribution<float> dist( minInclusive, nextafter( maxInclusive, FLT_MAX ) );
	return dist( randNumGenerator );
}

int Utility::Random( int minInclusive, int maxInclusive )
{
	if ( minInclusive > maxInclusive )
	{
		int temp = minInclusive;
		minInclusive = maxInclusive;
		maxInclusive = temp;
	}

	std::random_device randDevice;
	std::mt19937 randNumGenerator( randDevice() );
	std::uniform_int_distribution<> dist( minInclusive, maxInclusive );
	return dist( randNumGenerator );
}

bool Utility::RandomBool()
{
	return Random( 0, 1 ) == 1;
}

float Utility::DegreesToRadians( float angle )
{
	return static_cast<float>( angle * M_PI / 180 );
}

float Utility::RadiansToDegrees( float radians )
{
	return static_cast<float>( radians * 180 / M_PI );
}

float Utility::Clamp( float value, float min, float max )
{
	if ( value < min )
	{
		value = min;
	}

	if ( value > max )
	{
		value = max;
	}

	return value;
}

float Utility::Clamp01( float value )
{
	return Clamp( value, 0.0f, 1.0f );
}

int Utility::Clamp( int value, int min, int max )
{
	if ( value < min )
	{
		value = min;
	}

	if ( value > max )
	{
		value = max;
	}

	return value;
}

float Utility::Lerp( float from, float to, float percent )
{
	percent = Clamp01( percent );

	float distance = to - from;

	return from + distance * percent;
}

int Utility::PositiveMod( int num, int denom )
{
	return ( num % denom + denom ) % denom;
}

float Utility::PositiveMod( float num, float denom )
{
	return fmodf( fmodf( num, denom ) + denom, denom );
}

Color::Color( float red, float green, float blue, float alpha )
{
	this->red = Utility::Clamp01( red );
	this->green = Utility::Clamp01( green );
	this->blue = Utility::Clamp01( blue );
	this->alpha = Utility::Clamp01( alpha );
}

Color::Color( int red, int green, int blue, int alpha )
{
	this->red = Utility::Clamp01( red / 255.0f );
	this->green = Utility::Clamp01( green / 255.0f );
	this->blue = Utility::Clamp01( blue / 255.0f );
	this->alpha = Utility::Clamp01( alpha / 255.0f );
}

Vector2::Vector2( float x, float y )
{
	this->x = x;
	this->y = y;
}

float Vector2::Magnitude() const
{
	return sqrtf( powf( x, 2 ) + powf( y, 2 ) );
}

Vector2 Vector2::Scale( float scalarModifier )
{
	x *= scalarModifier;
	y *= scalarModifier;
	return *this;
}

Vector2 Vector2::Normalize()
{
	return Scale( 1 / Magnitude() );
}

Vector2 Vector2::Translate( float inX, float inY )
{
	x += inX;
	y += inY;
	return *this;
}

Vector3::Vector3( float x, float y, float z )
{
	this->x = x;
	this->y = y;
	this->z = z;
}

float Vector3::Magnitude() const
{
	return sqrtf( powf( x, 2 ) + powf( y, 2 ) + powf( z, 2 ) );
}

Vector3 Vector3::Scale( float scalarModifier )
{
	x *= scalarModifier;
	y *= scalarModifier;
	z *= scalarModifier;
	return *this;
}

// Modifies the vector in question
Vector3 Vector3::Normalize()
{
	return Scale( 1 / Magnitude() );
}

// Does not modify the vector in question
Vector3 Vector3::Normalize( Vector3 vector )
{
	return vector.Normalize();
}

float Vector3::DistanceSquared( Vector3 a, Vector3 b )
{
	return pow( a.x - b.x, 2 ) + pow( a.y - b.y, 2 ) + pow( a.z - b.z, 2 );
}

float Vector3::Distance( Vector3 a, Vector3 b )
{
	return sqrtf( DistanceSquared( a, b ) );
}

Vector3 Vector3::Translate( float inX, float inY, float inZ )
{
	x += inX;
	y += inY;
	z += inZ;
	return *this;
}

Vector3 Vector3::operator-( const Vector3& other ) const
{
	return Vector3( x - other.x, y - other.y, z - other.z );
}

Vector3 Vector3::operator-=( const Vector3& other )
{
	*this = *this - other;
	return *this;
}

Vector3 Vector3::operator+( const Vector3& other ) const
{
	return Vector3( x + other.x, y + other.y, z + other.z );
}

Vector3 Vector3::operator+=( const Vector3& other )
{
	*this = *this + other;
	return *this;
}

Vector3 Vector3::operator*( float scalar ) const
{
	return Vector3( x * scalar, y * scalar, z* scalar );
}

Vector3 Vector3::operator*=( float scalar )
{
	*this = *this * scalar;
	return *this;
}

Vector3 Vector3::Cross( Vector3 a, Vector3 b )
{
	return Vector3( a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x );
}

//Vector3 Vector3::Rotate( Vector3 primary, Vector3 rotation )
//{
//	// http://stackoverflow.com/questions/14607640/rotating-a-vector-in-3d-space
//	return NULL;
//}

Vector3 Vector3::ClampMagnitude( Vector3 vector, float maxMagnitude )
{
	if ( vector.Magnitude() <= maxMagnitude )
	{
		return vector;
	}
	else
	{
		return vector.Normalize().Scale( maxMagnitude );
	}
}