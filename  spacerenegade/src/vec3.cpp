#include <sstream>
#include "vec3.h"


////////////////////////////////////////////////////////////////////////////////
// ----------------------------- Vector ------------------------------------- //
////////////////////////////////////////////////////////////////////////////////

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
string Vec3::str()
{
	std::ostringstream rtn;
	rtn.precision(5);
	rtn << '[' << _x << ", " << _y << ", " << _z << ']';
	return rtn.str();
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
Vec3 Vec3::operator-=(const Vec3& a)
{
	this->_x -= a._x;
	this->_y -= a._y;
	this->_z -= a._z;

	return *this;
}

const Vec3 operator*(const GLdouble *m, const Vec3& v)
{
	double denom = m[3]*v.x() + m[7]*v.y() + m[11]*v.z() + m[15];
	return Vec3((m[0]*v.x() + m[4]*v.y() + m[8]*v.z() + m[12])/denom,
	            (m[1]*v.x() + m[5]*v.y() + m[9]*v.z() + m[13])/denom,
				(m[2]*v.x() + m[6]*v.y() + m[10]*v.z() + m[14])/denom);
}

// Multiply a vector by a scalar and return it.
Vec3 Vec3::operator*=(const double a)
{
	this->_x *= a;
	this->_y *= a;
	this->_z *= a;

	return *this;
}

// Multiply a vector by a scalar and return it.
Vec3 Vec3::operator/=(const double a)
{
	this->_x /= a;
	this->_y /= a;
	this->_z /= a;

	return *this;
}

// Multiply a scalar by each component of the vector.
const Vec3 operator*(const double a, const Vec3& b)
{
	return b * a;
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



////////////////////////////////////////////////////////////////////////////////
// ------------------------------ Matrix ------------------------------------ //
////////////////////////////////////////////////////////////////////////////////

string Matrix::str() const
{
	std::stringstream str;
	for (int i = 0; i < 16; i += 4) {
		str << "[ ";
		for (int j = 0; j < 4; j++)
			str << matrix[i+j] << ' ';
		str << "]\n";
	}
	return str.str();
}

Matrix Matrix::operator*(const Matrix& m) const
{
	GLdouble out[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	for (int i = 0; i < 16; i += 4)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				out[i+j] += matrix[j+4*k] * m[i+k];

	return Matrix(out);
}

Matrix Matrix::operator*(const GLdouble m[16]) const
{
	GLdouble out[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	for (int i = 0; i < 16; i += 4)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				out[i+j] += matrix[j+4*k] * m[i+k];

	return Matrix(out);
}

Matrix& Matrix::operator*=(const Matrix& m)
{
	*this = *this * m;
	return *this;
}

Matrix& Matrix::operator*=(const GLdouble m[16])
{
	*this = *this * m;
	return *this;
}

Matrix& Matrix::operator=(const Matrix& m)
{
	for (int i = 0; i < 16; i++)
		matrix[i] = m[i];
	return *this;
}

Matrix& Matrix::operator=(const GLdouble m[16])
{
	for (int i = 0; i < 16; i++)
		matrix[i] = m[i];
	return *this;
}

