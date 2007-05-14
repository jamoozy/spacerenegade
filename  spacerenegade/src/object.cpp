#include "object.h"
#include "vec3.h"
#include "environment.h"

extern OctTree *env;

// Create a new obect at the origin that does not move.
Object::Object() : leaf(NULL), position(0,0,0), velocity(0,0,0) {}

// Creates a new object at the given point with the given velocity.
Object::Object(double px, double py, double pz, double vx, double vy, double vz) :
	leaf(NULL), position(px,py,pz), velocity(vx,vy,vz) {}

// Creates a new object at the given point with the given velocity.
Object::Object(const Vec3& pos, const Vec3& v) : position(pos), velocity(v) {}

// Does nothing (nothing to be done).
Object::~Object() {}

// Updates the position.
void Object::draw()
{
	position += velocity;
}

// Get the position of this object.
Vec3 Object::getPos()
{
	return position;
}

// Get the position of the object in the next frame.
Vec3 Object::getNextPos()
{
	return position + velocity;
}

// Make sure we're still in the right place.
void Object::checkResidence()
{
	if (position.x() < leaf->min.x() || leaf->max.x() < position.x() ||
		position.y() < leaf->min.y() || leaf->max.y() < position.y() ||
		position.z() < leaf->min.z() || leaf->max.z() < position.z())
	{
		leaf->remove(this);
		env->add(this);
	}
}

// Sets
void Object::setResidence(Leaf *l)
{
	leaf = l;
}

Leaf *Object::getResidence()
{
	return leaf;
}

