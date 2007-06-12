
#ifndef BUILDCOMPONENT_H
#define BUILDCOMPONENT_H

#include "object.h"

class Material : public Object
{
public:
	Material(const Vec3& pos, const Vec3& vel) : Object("", pos, vel, 0,0,1) { radius = 1.5; };
	virtual ~Material() {};
	virtual string getType() { return "Material"; };

	virtual void draw(int pass);
	virtual void hits() {};
};

#endif

