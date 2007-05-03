#include "vec3.h"


// The constructor initializes the vector to the origin.
Vec3::Vec3() : _x(0), _y(0), _z(0) {}

// This constructor initializes the vector to the given point.
Vec3::Vec3(int a, int b, int c) : _x(a), _y(b), _z(c) {}

// This constructor initializes the vector to another vector.
Vec3::Vec3(const Vec3& a) : _x(a._x), _y(a._y), _z(a._z) {}

// Get the coordinates of this vector.
int Vec3::x() { return _x; }
int Vec3::y() { return _y; }
int Vec3::z() { return _z; }

// Add two vectors.
const Vec3 Vec3::operator+(const Vec3& a)
{
	return Vec3(a._x + _x, a._y + _y, a._z + _z);
}

// Subtract two vectors.
const Vec3 Vec3::operator-(const Vec3& a)
{
	return Vec3(_x - a._x, _y - a._y, _z - a._z);
}

// Dot product of two vectors.
const Vec3 Vec3::operator*(const Vec3& a)
{
	return Vec3(_y * a._z - _z * a._y, _z * a._x - _x * z._z, _x * a._y, - _y * a._x);
}

// Cross product of two vectors.
const Vec3 Vec3::operator^(const Vec3& a)
{
	return Vec3(_x - a._x, _y - a._y, _z - a._z);
}

// Assign one vector to another (copy it).
Vec3& Vec3::operator=(const Vec3& a)
{
	if (this == &a)
		return *this;

	_x = a._x;
	_y = a._y;
	_z = a._z;

	return *this;
}
