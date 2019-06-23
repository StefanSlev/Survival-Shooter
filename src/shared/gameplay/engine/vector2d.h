#ifndef SHARED_GAMEPLAY_ENGINE_VECTOR2D
#define SHARED_GAMEPLAY_ENGINE_VECTOR2D

#include "shared/gameplay/gameplay_base.h"

// -------------------------------------------------------------------------------------
// Class with basic 2D vector functionality.
// Has multiple versions of constructors and operators implemented, for ease of use.
// Note that the +=, -=, *=, /= operators are more efficient than their counterparts.
// Other methods: Dot, Cross, LengthSq, Length, SafeNormalize.
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class Vector2D
{
public:
#pragma region Constructors
	// Default constructor (contents not initialized)
	Vector2D();
	// Constructor with x and y coordinates
	Vector2D(float x, float y);
	// Both x and y will have the same value
	Vector2D(float xy);
	// Copy constructor
	Vector2D(const Vector2D& v);
#pragma endregion

#pragma region Operators
	// Set X and Y
	const Vector2D& Set(float x, float y);

	// Different versions of operator =
	const Vector2D& operator=(float xy);
	const Vector2D& operator=(const Vector2D& v);

	// Different versions of operator +=
	const Vector2D& operator+=(float xy);
	const Vector2D& operator+=(const Vector2D& v);

	// Different versions of operator -=
	const Vector2D& operator-=(float xy);
	const Vector2D& operator-=(const Vector2D& v);

	// Different versions of operator *=
	const Vector2D& operator*=(float xy);
	const Vector2D& operator*=(const Vector2D& v);

	// Different versions of operator /=
	const Vector2D& operator/=(float xy);
	const Vector2D& operator/=(const Vector2D& v);

	// Different versions of operator + (usually less performant than +=)
	const Vector2D operator+(float xy)const;
	const Vector2D operator+(const Vector2D& v)const;

	// Different versions of operator - (usually less performant than -=)
	const Vector2D operator-(float xy)const;
	const Vector2D operator-(const Vector2D& v)const;

	// Different versions of operator * (usually less performant than *=)
	const Vector2D operator*(float xy)const;
	const Vector2D operator*(const Vector2D& v)const;

	// Different versions of operator / (usually less performant than /=)
	const Vector2D operator/(float xy)const;
	const Vector2D operator/(const Vector2D& v)const;
#pragma endregion

#pragma region Vector operations
	// Dot product
	float Dot(const Vector2D& v)const;
	// Cross product
	float Cross(const Vector2D& v)const;
	// Squared length
	float LengthSq()const;
	// Length
	float Length()const;
	// Safe normalize (returns false if lengthSq < epsSq)
	bool SafeNormalize(float epsSq = 1e-8f);
	// Roate the vector anticlockwise
	const Vector2D Rotate(float angleDegree) const;
#pragma endregion
	
#pragma region Members
	float x;
	float y;
#pragma endregion
};

#pragma region Binary operators
// These binary operators are usually less performant than their $= counterparts
inline const Vector2D operator+(float xy, const Vector2D& v)	{ return v + xy; }
inline const Vector2D operator-(float xy, const Vector2D& v)	{ return v - xy; }
inline const Vector2D operator*(float xy, const Vector2D& v)	{ return v * xy; }
inline const Vector2D operator/(float xy, const Vector2D& v)	{ return v / xy; }
#pragma endregion

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ENGINE_VECTOR2D