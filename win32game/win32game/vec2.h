#pragma once

#include <cmath>
using namespace std;

/**
* 2 dimensional (math) vector
*/
template<typename TYPE>

struct Vec2
{
	/** the x, y coordinates */
	TYPE x, y;

	/** initializes the Vec2 */
	Vec2(TYPE x, TYPE y) :x(x), y(y) {}

	/** default constructor - sets x,y to 0,0 */
	Vec2() :x(0), y(0) {}

	/** @return true if the given x, y coordinate matches this Vec2's data */
	bool is(TYPE x, TYPE y) const;

	/** @return true if the given x, y coordinate matches this Vec2's data */
	bool is(Vec2 xy) const;

	/** @return true if the Vec2 is within the given rectangular boundary */
	bool isWithin(TYPE minx, TYPE miny, TYPE maxx, TYPE maxy);

	/** re-initialize */
	void init(TYPE x, TYPE y);

	/** @return a random Vec2 within the specified boundary */
	static Vec2 random(Vec2 min, Vec2 max);

	/** @return pythagorean distance from the origin */
	float magnitude();

	Vec2 operator+(Vec2 const & v) const;
	Vec2 operator-(Vec2 const & v) const;
	Vec2 operator*(TYPE value) const;
	Vec2 operator/(TYPE value) const;
	Vec2 & operator+=(Vec2& v);
	Vec2 & operator-=(Vec2& v);
	Vec2 & operator*=(TYPE value);
	Vec2 & operator/=(TYPE value);
	bool operator==(Vec2 const & v) const { return is(v); }
	bool operator!=(Vec2 const & v) const { return !is(v); }

	/**
	* @param A,B line 1
	* @param C,D line 2
	* @param point __OUT to the intersection of line AB and CD
	* @param dist __OUT the distance along line AB to the intersection
	* @return true if intersection occurs between the lines
	*/
	static bool lineIntersection(const Vec2 & A, const Vec2 & B,
		const Vec2 & C, const Vec2 & D,
		float & dist, Vec2 & point);

	/**
	* @return positive if v2 is clockwise of this vector
	* (assume Y points down, X to right)
	*/
	float sign(const Vec2 & v) const;

	/** @return true if this point is inside the given triangle */
	bool isInsideTriangle(Vec2 const & a, Vec2 const & b,
		Vec2 const & c) const;

	void limitMagnitude(float max);

};


/** @return true if the given x, y coordinate matches this Vec2's data */
template<typename TYPE>
bool Vec2<TYPE>::is(TYPE x, TYPE y) const
{
	return this->x == x && this->y == y;
}

/** @return true if the given x, y coordinate matches this Vec2's data */
template<typename TYPE>
bool Vec2<TYPE>::is(Vec2 xy) const
{
	return xy.x == x && xy.y == y;
}

/** @return true if the Vec2 is within the given rectangular boundary */
template<typename TYPE>
bool Vec2<TYPE>::isWithin(TYPE minx, TYPE miny, TYPE maxx, TYPE maxy)
{
	return x >= minx && y >= miny && x < maxx && y < maxy;
}

/** re-initialize */
template<typename TYPE>
void Vec2<TYPE>::init(TYPE a_x, TYPE a_y)
{
	x = a_x;
	y = a_y;
}

/** @return pythagorean distance from the origin */
template<typename TYPE>
float Vec2<TYPE>::magnitude()
{
	return (std::sqrt(x*x + y*y));
}

template<typename TYPE>
Vec2<TYPE> Vec2<TYPE>::operator+(Vec2 const & v) const { return Vec2(x + v.x, y + v.y); }

template<typename TYPE>
Vec2<TYPE> Vec2<TYPE>::operator-(Vec2 const & v) const { return Vec2(x - v.x, y - v.y); }

template<typename TYPE>
Vec2<TYPE> Vec2<TYPE>::operator*(TYPE value) const { return Vec2(x*value, y*value); }

template<typename TYPE>
Vec2<TYPE> Vec2<TYPE>::operator/(TYPE value) const { return Vec2((TYPE)x / value, (TYPE)y / value); }

template<typename TYPE>
Vec2<TYPE> & Vec2<TYPE>::operator+=(Vec2& v) { x += v.x;   y += v.y;   return *this; }

template<typename TYPE>
Vec2<TYPE> & Vec2<TYPE>::operator-=(Vec2& v) { x -= v.x;   y -= v.y;   return *this; }

template<typename TYPE>
Vec2<TYPE> & Vec2<TYPE>::operator*=(TYPE value) { x *= value; y *= value; return *this; }

template<typename TYPE>
Vec2<TYPE> & Vec2<TYPE>::operator/=(TYPE value) { x /= value; y /= value; return *this; }

#include "stdlib.h"

template<typename TYPE>
inline TYPE randInUnitInterval()
{
	const int RANDMAX = 32768;
	return (rand() % RANDMAX) / ((float)RANDMAX);
}

/** @return a random Vec2 within the specified boundary */

template<typename TYPE>
Vec2<TYPE> Vec2<TYPE>::random(Vec2 min, Vec2 max)
{
	TYPE w = max.x - min.x, h = max.y - min.y;
	return Vec2<TYPE>(
		randInUnitInterval() * w + min.x,
		randInUnitInterval() * h + min.y);
}

/**
* @param A,B line 1
* @param C,D line 2
* @param point __OUT to the intersection of line AB and CD
* @param dist __OUT the distance along line AB to the intersection
* @return true if intersection occurs between the lines
*/

template<typename TYPE>
bool Vec2<TYPE>::lineIntersection(const Vec2 & A, const Vec2 & B,
	const Vec2 & C, const Vec2 & D,
	float & dist, Vec2 & point)
{
	float rTop = (A.y - C.y)*(D.x - C.x) - (A.x - C.x)*(D.y - C.y);
	float rBot = (B.x - A.x)*(D.y - C.y) - (B.y - A.y)*(D.x - C.x);
	float sTop = (A.y - C.y)*(B.x - A.x) - (A.x - C.x)*(B.y - A.y);
	float sBot = (B.x - A.x)*(D.y - C.y) - (B.y - A.y)*(D.x - C.x);
	if ((rBot == 0) || (sBot == 0))
	{
		//lines are parallel
		return false;
	}
	float r = rTop / rBot;
	float s = sTop / sBot;
	Vec2 delta = B - A;
	dist = delta.magnitude() * r;
	point = A + (delta * r);
	return ((r > 0) && (r < 1) && (s > 0) && (s < 1));
}

/**
* @return positive if v2 is clockwise of this vector
* (assume Y points down, X to right)
*/

template<typename TYPE>
float Vec2<TYPE>::sign(const Vec2 & v) const
{
	return (x*v.y) - (y*v.x);
}

/** @return true if this point is inside the given triangle */
template<typename TYPE>
bool Vec2<TYPE>::isInsideTriangle(Vec2 const & a, Vec2 const & b,
	Vec2 const & c) const
{
	float signab = (*this - a).sign(b - a),
		signbc = (*this - b).sign(c - b),
		signac = (*this - c).sign(a - c);
	return(((signab >= 0) == (signbc >= 0)) && ((signbc >= 0) == (signac >= 0)))
		|| (((signab <= 0) == (signbc <= 0)) && ((signbc <= 0) == (signac <= 0)));
}

template<typename TYPE>
void Vec2<TYPE>::limitMagnitude(float max)
{
	float m = magnitude();
	if (m > max)
	{
		x = x * max / m;
		y = y * max / m;
	}
}
