#include <sstream>
#include "vec3.h"


////////////////////////////////////////////////////////////////////////////////
// ----------------------------- Vector ------------------------------------- //
////////////////////////////////////////////////////////////////////////////////

// Make this vector's length 1.
void Vec3::normalize()
{
	double norm = sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);

	if (norm == 0) return;

	vec[0] /= norm;
	vec[1] /= norm;
	vec[2] /= norm;
}

// Returns a string representation of this vector.
string Vec3::str()
{
	std::ostringstream rtn;
	rtn.precision(5);
	rtn << '[' << vec[0] << ", " << vec[1] << ", " << vec[2] << ']';
	return rtn.str();
}

// Add two vectors.
Vec3 Vec3::operator+=(const Vec3& a)
{
	this->vec[0] += a.vec[0];
	this->vec[1] += a.vec[1];
	this->vec[2] += a.vec[2];

	return *this;
}

// Subtract two vectors.
Vec3 Vec3::operator-=(const Vec3& a)
{
	this->vec[0] -= a.vec[0];
	this->vec[1] -= a.vec[1];
	this->vec[2] -= a.vec[2];

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
	this->vec[0] *= a;
	this->vec[1] *= a;
	this->vec[2] *= a;

	return *this;
}

// Multiply a vector by a scalar and return it.
Vec3 Vec3::operator/=(const double a)
{
	this->vec[0] /= a;
	this->vec[1] /= a;
	this->vec[2] /= a;

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

	vec[0] = a.vec[0];
	vec[1] = a.vec[1];
	vec[2] = a.vec[2];

	return *this;
}



////////////////////////////////////////////////////////////////////////////////
// ------------------------------ Matrix ------------------------------------ //
////////////////////////////////////////////////////////////////////////////////

string Matrix::str() const
{
	std::stringstream str;
	for (int i = 0; i < 4; i++) {
		str << "[ ";
		for (int j = 0; j < 16; j += 4)
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

