#ifndef ASTEROID_H
#define ASTEROID_H

#include "object.h"
#include "vec3.h"

class Asteroid : public Object
{
private:
	Vec3 angle;
	Vec3 avelocity;

public:
	Asteroid();
	Asteroid(double px, double py, double pz, double vx, double vy, double vz);
	Asteroid(const Vec3& pos, const Vec3& v);
	void draw();
};

#endif
