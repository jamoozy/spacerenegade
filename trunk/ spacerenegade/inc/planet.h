#ifndef PLANET_H
#define PLANET_H

#include "Model_3DS.h"
#include "object.h"
#include "vec3.h"

class Planet : public Object
{
private:

	void setRadius(double r);

public:
	Planet(double radius, const Vec3& pos);
	virtual ~Planet() {};
	virtual std::string getType() const { return "Planet"; };

	virtual void draw();
	virtual void hits(Object *o);
	virtual bool canLandOn() { return true; };
};

#endif

