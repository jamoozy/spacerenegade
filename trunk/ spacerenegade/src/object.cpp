#include <iostream>
#include "object.h"
#include "vec3.h"
#include "environment.h"

extern OctTree *env;

// Create a new obect at the origin that does not move.
Object::Object(char *modelName) : leaf(NULL), radius(4.0), position(0,0,0), velocity(0,0,0), damage(0)
{
	if (modelName != NULL && modelName[0] != '\0')
		modelLoaded = model.Load(modelName);
}

// Creates a new object at the given point with the given velocity.
Object::Object(char *modelName, double px, double py, double pz, double vx, double vy, double vz) :
	leaf(NULL), radius(4.0), position(px,py,pz), velocity(vx,vy,vz), damage(0)
{
	if (modelName != NULL && modelName[0] != '\0')
		modelLoaded = model.Load(modelName);
}

// Creates a new object at the given point with the given velocity.
Object::Object(char *modelName, const Vec3& pos, const Vec3& v) :
	leaf(NULL), radius(4.0), position(pos), velocity(v), damage(0)
{
	if (modelName != NULL && modelName[0] != '\0')
		modelLoaded = model.Load(modelName);
}

// Does nothing (nothing to be done).
Object::~Object() {}

// Make sure we're still in the right place.
bool Object::checkResidence()
{
	if (position.x() < leaf->minBound.x() || leaf->maxBound.x() < position.x() ||
		position.y() < leaf->minBound.y() || leaf->maxBound.y() < position.y() ||
		position.z() < leaf->minBound.z() || leaf->maxBound.z() < position.z())
	{
		leaf->remove(this);
		env->add(this);
		return false;
	}

	return true;
}

// Determines if the passed object collides with this one.
bool Object::collidesWith(Object *o)
{
	Vec3 diff(o->position - position);
	double distance2 = diff * diff;
	double sum = o->radius + radius;
	return (distance2 < sum * sum);
}

void Object::hits(Object *o)
{
	// TODO: Put PM's collision code here.
}

