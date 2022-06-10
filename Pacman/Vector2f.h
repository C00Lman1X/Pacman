#ifndef VECTOR2F_H
#define VECTOR2F_H

#include <math.h> 

struct TileCoord;

class Vector2f
{
public:

	Vector2f()
	{
		myX = 0.f;
		myY = 0.f;
	}

	Vector2f(float anX, float anY)
	{
		myX = anX;
		myY = anY;
	}

	Vector2f(int anX, int anY)
	{
		myX = anX;
		myY = anY;
	}

	Vector2f(const TileCoord& tile);

	bool operator==(const Vector2f &other) const
	{
		return myX == other.myX && myY == other.myY;
	}
	
	bool operator!=(const Vector2f &other) const
	{
		return !(*this == other);
	}

	const Vector2f operator-(const Vector2f &other) const 
	{
		Vector2f v(myX - other.myX, myY - other.myY);
		return v;
	}

	const Vector2f operator+(const Vector2f &other) const 
	{
		Vector2f v(myX + other.myX, myY + other.myY);
		return v;
	}

	
	const Vector2f operator*(const Vector2f& other) const 
	{
		Vector2f v(myX*other.myX, myY*other.myY);
		return v;
	}

	Vector2f& operator+=(const Vector2f &other) 
	{
		myX = myX + other.myX;
		myY = myY + other.myY;

		return *this;
	}

	Vector2f& operator*=(const float aFloat) 
	{
		myX *= aFloat;
		myY *= aFloat;

		return *this;
	}

	Vector2f& operator/=(const float aFloat) 
	{
		myX /= aFloat;
		myY /= aFloat;

		return *this;
	}

	const Vector2f operator*(const float aValue) const 
	{
		Vector2f v(myX * aValue, myY * aValue);
		return v;
	}

	float Length() const
	{
		return sqrt(myX*myX + myY*myY);
	}

	void Normalize()
	{
		float length = Length();

		if (length > 0.f)
			*this /= length;
	}

	float myX;
	float myY;
};

#endif // VECTOR2F_H