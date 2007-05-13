#ifndef ASTEROID_H
#define ASTEROID_H

#include "object.h"
#include "vec3.h"

class Asteroid : public Object
{
private:
	Vec3 angle;      // The angle of rotation in the local coord-sys.
	Vec3 avelocity;  // The change of angle over time.

public:
	Asteroid();
	Asteroid(double px, double py, double pz, double vx, double vy, double vz);
	Asteroid(const Vec3& pos, const Vec3& v);
	void draw();
};

#endif
