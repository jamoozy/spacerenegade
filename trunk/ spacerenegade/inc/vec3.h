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
	Vec3(double a, double b, double c);
	Vec3(const Vec3& a);

	// Coordinates of this object.
	double x();
	double y();
	double z();

	const std::string str();
		
	const Vec3 operator+(const Vec3& a);  // element-wise sum
	Vec3 operator+=(const Vec3& a); // element-wise sum
	const Vec3 operator-(const Vec3& a);  // element-wise difference
	Vec3 operator-=(const Vec3& a); // element-wise difference
	const Vec3 operator*(const Vec3& a);  // dot product
	const Vec3 operator^(const Vec3& a);  // cross product
	Vec3& operator=(const Vec3& a);
};

#endif
