#ifndef VEC3_H
#define VEC3_H

class Vec3
{
private:
	int _x;
	int _y;
	int _z;

public:

	Vec3();
	Vec3(int a, int b, int c);
	Vec3(const Vec3 a);

	int x();
	int y();
	int z();
		
	const Vec3 operator+(const Vec3& a); // element-wise sum
	const Vec3 operator-(const Vec3& a); // element-wise difference
	const Vec3 operator*(const Vec3& a); // dot product
	const Vec3 operator^(const Vec3& a); // cross product

	Vec3& operator=(const Vec3& a);
};

#endif
