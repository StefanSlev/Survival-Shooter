#include "shared/gameplay/engine/vector2d.h"
#include "shared/gameplay/engine/constants.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

#pragma region Constructors

// Default constructor (contents not initialized)
Vector2D::Vector2D()
{
}

// Constructor with x and y coordinates
Vector2D::Vector2D(float x_, float y_)
: x(x_)
, y(y_)
{
}

// Both x and y will have the same value
Vector2D::Vector2D(float xy)
: x(xy)
, y(xy)
{
}

// Copy constructor
Vector2D::Vector2D(const Vector2D& v)
: x(v.x)
, y(v.y)
{
}
#pragma endregion


#pragma region Operators
// Set X and Y
const Vector2D& Vector2D::Set(float x_, float y_)
{
	x = x_;
	y = y_;
	return *this;
}

// Different versions of operator =
const Vector2D& Vector2D::operator=(float xy)
{
	x = y = xy;
	return *this;
}
const Vector2D& Vector2D::operator=(const Vector2D& v)
{
	x = v.x;
	y = v.y;
	return *this;
}

// Different versions of operator +=
const Vector2D& Vector2D::operator+=(float xy)
{
	x += xy;
	y += xy;
	return *this;
}
const Vector2D& Vector2D::operator+=(const Vector2D& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

// Different versions of operator -=
const Vector2D& Vector2D::operator-=(float xy)
{
	x -= xy;
	y -= xy;
	return *this;
}
const Vector2D& Vector2D::operator-=(const Vector2D& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

// Different versions of operator *=
const Vector2D& Vector2D::operator*=(float xy)
{
	x *= xy;
	y *= xy;
	return *this;
}
const Vector2D& Vector2D::operator*=(const Vector2D& v)
{
	x *= v.x;
	y *= v.y;
	return *this;
}

// Different versions of operator /=
const Vector2D& Vector2D::operator/=(float xy)
{
	float coef = 1.f / xy;
	x *= coef;
	y *= coef;
	return *this;
}
const Vector2D& Vector2D::operator/=(const Vector2D& v)
{
	x /= v.x;
	y /= v.y;
	return *this;
}

// Different versions of operator +
const Vector2D Vector2D::operator+(float xy)const
{
	Vector2D temp(*this);
	return temp += xy;
}
const Vector2D Vector2D::operator+(const Vector2D& v)const
{
	Vector2D temp(*this);
	return temp += v;
}

// Different versions of operator -
const Vector2D Vector2D::operator-(float xy)const
{
	Vector2D temp(*this);
	return temp -= xy;
}
const Vector2D Vector2D::operator-(const Vector2D& v)const
{
	Vector2D temp(*this);
	return temp -= v;
}

// Different versions of operator *
const Vector2D Vector2D::operator*(float xy)const
{
	Vector2D temp(*this);
	return temp *= xy;
}
const Vector2D Vector2D::operator*(const Vector2D& v)const
{
	Vector2D temp(*this);
	return temp *= v;
}

// Different versions of operator /
const Vector2D Vector2D::operator/(float xy)const
{
	Vector2D temp(*this);
	return temp /= xy;
}
const Vector2D Vector2D::operator/(const Vector2D& v)const
{
	Vector2D temp(*this);
	return temp /= v;
}
#pragma endregion

#pragma region Vector operations
// Dot product
float Vector2D::Dot(const Vector2D& v)const
{
	return (x * v.x) + (y * v.y);
}

// Cross product
float Vector2D::Cross(const Vector2D& v)const
{
	return (x * v.y) - (y * v.x);
}

// Squared length
float Vector2D::LengthSq()const
{
	return (x * x) + (y * y);
}

// Length
float Vector2D::Length()const
{
	return sqrtf((x * x) + (y * y));
}

// Safe normalize
bool Vector2D::SafeNormalize(float epsSq)
{
	float lengthSq = LengthSq();
	if(lengthSq < epsSq)
		return false;
	float coef = 1.f / sqrtf(lengthSq);
	x *= coef;
	y *= coef;
	return true;
}
// Rotate the vector
const Vector2D Vector2D::Rotate(float angleDegree)const 
{
	Vector2D temp;
	double fSinA = sin(angleDegree * M_PI / 180);
	double fCosA = cos(angleDegree * M_PI / 180);
	temp.x = (float)(fCosA * x - fSinA * y);
	temp.y = (float)(fSinA * x + fCosA * y);
	return temp;
}

#pragma endregion

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE