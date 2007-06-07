#ifndef PLANET_H
#define PLANET_H

#include "Model_3DS.h"
#include "object.h"
#include "vec3.h"

class Planet : public Object
{
private:

public:
	Planet(const Vec3& pos, double radius);
	virtual ~Planet() {};
	virtual std::string getType() const { return "Planet"; };

	virtual void update() {};
	virtual void draw(int pass);
	virtual void hits(Object *o);
	virtual bool canLandOn() { return true; };
};

#endif

