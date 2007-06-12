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
	double vec[3];

public:

	// The constructor initializes the vector to the origin.
	Vec3() { vec[0] = vec[1] = vec[2] = 0; };
	// This constructor initializes the vector to another vector.
	Vec3(const Vec3& a) { for (int i = 0; i < 3; i++) vec[i] = a.vec[i]; };
	// This constructor initializes the vector to the given point.
	Vec3(double a, double b, double c) { vec[0] = a; vec[1] = b; vec[2] = c; };

	// Coordinates of this object.
	double x() const { return vec[0]; };
	double y() const { return vec[1]; };
	double z() const { return vec[2]; };
	const double *array() const { return vec; };

	double norm() const  // The length of the vector
	{ return sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]); }
	void normalize();  // Maintain direction but make length 1

	string str();  // String representation: [x, y, z]
		
	// Addition and Subtraction by another Vec3.
	Vec3 operator+(const Vec3& a) const  // element-wise sum
	{ return Vec3(a.vec[0] + vec[0], a.vec[1] + vec[1], a.vec[2] + vec[2]); }
	Vec3 operator-(const Vec3& a) const  // element-wise difference
	{ return Vec3(vec[0] - a.vec[0], vec[1] - a.vec[1], vec[2] - a.vec[2]); };
	Vec3 operator+=(const Vec3& a); // element-wise sum
	Vec3 operator-=(const Vec3& a); // element-wise difference

	// Addition and Subtraction by a scalar.
	Vec3 operator-(double a) const
	{ return Vec3(vec[0] - a, vec[1] - a, vec[2] - a); };

	// Vector Multiplications.
	const double operator*(const Vec3& a) const  // dot product
	{ return vec[0] * a.vec[0] + vec[1] * a.vec[1] + vec[2] * a.vec[2]; };
	Vec3 operator^(const Vec3& a) const  // cross product
	{ return Vec3(vec[1] * a.vec[2] - vec[2] * a.vec[1], vec[2] * a.vec[0] - vec[0] * a.vec[2], vec[0] * a.vec[1] - vec[1] * a.vec[0]); };

	// Scalar products and dividends.
	Vec3 operator*(double a) const  // scalar product
	{ return Vec3(vec[0] * a, vec[1] * a, vec[2] * a); };
	Vec3 operator/(double a) const // scalar dividend
	{ return Vec3(vec[0] / a, vec[1] / a, vec[2] / a); };
	Vec3 operator*=(double a); // scalar product
	Vec3 operator/=(double a); // scalar dividend

	// Bit-wise products and dividends.
	Vec3 operator/(const Vec3& a)
	{ return Vec3(vec[0] / a.vec[0], vec[1] / a.vec[1], vec[2] / a.vec[2]); };

	// Other.
	Vec3 operator-() const  // Unary negation.
	{ return Vec3(-vec[0], -vec[1], -vec[2]); };
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
	// Sets the matrix to the passed values.
//	Matrix(GLdouble a0, GLdouble a1, GLdouble a2, GLdouble a3,
//		   GLdouble a4, GLdouble a5, GLdouble a6, GLdouble a7,
//		   GLdouble a8, GLdouble a9, GLdouble a10, GLdouble a11,
//		   GLdouble a12, GLdouble a13, GLdouble a14, GLdouble a15)
//	{ for (int i = 0; i < 16; i++) matrix[i] = m[i]; };

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

