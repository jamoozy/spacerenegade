#ifndef VEC3_H
#define VEC3_H

#include <sstream>
#include <string>

class Vec3
{
private:
	double _x;
	double _y;
	double _z;

public:

	Vec3();
	Vec3(const double a, const double b, const double c);
	Vec3(const Vec3& a);

	// Coordinates of this object.
	double x();
	double y();
	double z();

	void normalize();  // Maintain direction but make length 1

	const std::string str();  // String representation: [x, y, z]
		
	const Vec3 operator+(const Vec3& a) const;  // element-wise sum
	Vec3 operator+=(const Vec3& a); // element-wise sum
	const Vec3 operator-(const Vec3& a) const;  // element-wise difference
	Vec3 operator-=(const Vec3& a); // element-wise difference
	const double operator*(const Vec3& a) const;  // dot product
	const Vec3 operator*(const double a) const;  // scalar product
	const Vec3 operator^(const Vec3& a) const;  // cross product
	Vec3& operator=(const Vec3& a);
};

const Vec3 operator*(const double a, const Vec3& b);

#endif
