#ifndef ASTEROID_H
#define ASTEROID_H

#include "Model_3DS.h"
#include "object.h"
#include "vec3.h"

class Asteroid : public Object
{
private:
	Vec3 angle;      // The angle of rotation in the local coord-sys.
	Vec3 avelocity;  // The change of angle over time.

	void setRadius(double r);
	Vec3 randPos() const;

public:
	Asteroid(double radius);
	Asteroid(double radius, double px, double py, double pz, double vx, double vy, double vz);
	Asteroid(double radius, const Vec3& pos, const Vec3& v);
	virtual ~Asteroid() {};
	virtual std::string getType() const { return "Asteroid"; };

	virtual void draw();
	virtual void hits(Object *o);
	virtual int maxHealth() const { return 10; }
};

#endif

