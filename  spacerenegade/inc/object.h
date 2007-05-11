#ifndef OBJECT_H
#define OBJECT_H

#include "vec3.h"

class Object
{
protected:
	Vec3 position;
	Vec3 velocity;

public:
	Object();
	Object(const Vec3& pos, const Vec3& v);
	Object(double px, double py, double pz, double vx, double vy, double vz);
	virtual ~Object();

	virtual void draw();
	virtual Vec3 getPos();
};
		
#endif
