#ifndef VEC3_H
#define VEC3_H

#include <GL/glut.h>
#include <sstream>
#include <string>
#include <cmath>

using std::string;

class Vec3
{
private:
	double _x;
	double _y;
	double _z;

public:

	// The constructor initializes the vector to the origin.
	Vec3() : _x(0), _y(0), _z(0) {}
	// This constructor initializes the vector to another vector.
	Vec3(const Vec3& a) : _x(a._x), _y(a._y), _z(a._z) {}
	// This constructor initializes the vector to the given point.
	Vec3(double a, double b, double c) : _x(a), _y(b), _z(c) {}

	// Coordinates of this object.
	double x() const { return _x; };
	double y() const { return _y; };
	double z() const { return _z; };

	double norm() const  // The length of the vector
	{ return sqrt(_x * _x + _y * _y + _z * _z); }
	void normalize();  // Maintain direction but make length 1

	string str();  // String representation: [x, y, z]
		
	// Addition and Subtraction.
	Vec3 operator+(const Vec3& a) const  // element-wise sum
	{ return Vec3(a._x + _x, a._y + _y, a._z + _z); }
	Vec3 operator-(const Vec3& a) const  // element-wise difference
	{ return Vec3(_x - a._x, _y - a._y, _z - a._z); };
	Vec3 operator+=(const Vec3& a); // element-wise sum
	Vec3 operator-=(const Vec3& a); // element-wise difference

	// Vector Multiplications.
	const double operator*(const Vec3& a) const  // dot product
	{ return _x * a._x + _y * a._y + _z * a._z; };
	Vec3 operator^(const Vec3& a) const  // cross product
	{ return Vec3(_y * a._z - _z * a._y, _z * a._x - _x * a._z, _x * a._y - _y * a._x); };

	// Scalar products and dividends.
	Vec3 operator*(double a) const  // scalar product
	{ return Vec3(_x * a, _y * a, _z * a); };
	Vec3 operator/(double a) const // scalar dividend
	{ return Vec3(_x / a, _y / a, _z / a); };
	Vec3 operator*=(double a); // scalar product
	Vec3 operator/=(double a); // scalar dividend

	// Other.
	Vec3 operator-() const  // Unary negation.
	{ return Vec3(-_x, -_y, -_z); };
	Vec3& operator=(const Vec3& a);  // Assignment.
};

class Matrix
{
private:
	GLdouble matrix[16];

public:
	// Sets the matrix to zero matrix.
	Matrix() { for (int i = 0; i < 16; i++) matrix[i] = 0; };
	// Sets the matrix to the passed matrix.
	Matrix(GLdouble m[16]) { for (int i = 0; i < 16; i++) matrix[i] = m[i]; };

	// Access elements.
	GLdouble operator[](int i) const // Access elements like an array.
	{ return matrix[i]; };
	const GLdouble *array() const // Get the array of elements in the matrix.
	{ return matrix; };

	Matrix operator*(const Matrix& m) const;
	Matrix operator*(const GLdouble m[16]) const;
	Matrix& operator*=(const Matrix& m);
	Matrix& operator*=(const GLdouble m[16]);
	Matrix& operator=(const Matrix& m);
	Matrix& operator=(const GLdouble m[16]);

	string str() const;
};

const Vec3 operator*(const double a, const Vec3& b);
const Vec3 operator*(const GLdouble *m, const Vec3& v); //matrix[4x4]*vec3(x,y,z,1)

#endif

