#include <sstream>
#include <cmath>
#include "vec3.h"


// The constructor initializes the vector to the origin.
Vec3::Vec3() : _x(0), _y(0), _z(0) {}

// This constructor initializes the vector to the given point.
Vec3::Vec3(const double a, const double b, const double c) : _x(a), _y(b), _z(c) {}

// This constructor initializes the vector to another vector.
Vec3::Vec3(const Vec3& a) : _x(a._x), _y(a._y), _z(a._z) {}

// Get the coordinates of this vector.
double Vec3::x() { return _x; }
double Vec3::y() { return _y; }
double Vec3::z() { return _z; }

// Find the length of the vector.
double Vec3::norm() const
{
	return sqrt(_x * _x + _y * _y + _z * _z);
}

// Make this vector's length 1.
void Vec3::normalize()
{
	double norm = sqrt(_x * _x + _y * _y + _z * _z);

	if (norm == 0) return;

	_x /= norm;
	_y /= norm;
	_z /= norm;
}

// Returns a string representation of this vector.
const std::string Vec3::str()
{
	std::ostringstream rtn;
	rtn.precision(5);
	rtn << '[' << _x << ", " << _y << ", " << _z << ']';
	return rtn.str();
}

// Add two vectors.
Vec3 Vec3::operator+(const Vec3& a) const
{
	return Vec3(a._x + _x, a._y + _y, a._z + _z);
}

// Add two vectors.
Vec3 Vec3::operator+=(const Vec3& a)
{
	this->_x += a._x;
	this->_y += a._y;
	this->_z += a._z;

	return *this;
}

// Subtract two vectors.
Vec3 Vec3::operator-(const Vec3& a) const
{
	return Vec3(_x - a._x, _y - a._y, _z - a._z);
}

// Subtract two vectors.
Vec3 Vec3::operator-=(const Vec3& a)
{
	this->_x -= a._x;
	this->_y -= a._y;
	this->_z -= a._z;

	return *this;
}

// Dot product of two vectors.
const double Vec3::operator*(const Vec3& a) const
{
	return _x * a._x + _y * a._y + _z * a._z;
}

// Multiply a scalar by each component of the vector.
const Vec3 Vec3::operator*(const double a) const
{
	return Vec3(_x * a, _y * a, _z * a);
}

// Multiply a vector by a scalar and return it.
const Vec3 Vec3::operator*=(const double a)
{
	this->_x *= a;
	this->_y *= a;
	this->_z *= a;

	return *this;
}

// Multiply a scalar by each component of the vector.
const Vec3 operator*(const double a, const Vec3& b)
{
	return b * a;
}

// Cross product of two vectors.
const Vec3 Vec3::operator^(const Vec3& a) const
{
	return Vec3(_y * a._z - _z * a._y, _z * a._x - _x * a._z, _x * a._y - _y * a._x);
}

// Unary negation.
const Vec3& Vec3::operator-() const
{
	return Vec3(-_x, -_y, -_z);
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
