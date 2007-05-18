#include "object.h"
#include "vec3.h"
#include "environment.h"

extern OctTree *env;

// Create a new obect at the origin that does not move.
Object::Object() : leaf(NULL), radius(4.0), radius2(16.0), position(0,0,0), velocity(0,0,0) {}

// Creates a new object at the given point with the given velocity.
Object::Object(double px, double py, double pz, double vx, double vy, double vz) :
	leaf(NULL), radius(4.0), radius2(16.0), position(px,py,pz), velocity(vx,vy,vz) {}

// Creates a new object at the given point with the given velocity.
Object::Object(const Vec3& pos, const Vec3& v) : leaf(NULL), radius(4.0), radius2(16.0), position(pos), velocity(v) {}

// Does nothing (nothing to be done).
Object::~Object() {}

// Updates the position.
void Object::draw()
{
	position += velocity;
}

// Sets the object at a new position.
void Object::setAt(const Vec3& pos)
{
	position = pos;
}

// Sets the object at a new position.
void Object::setAt(double x, double y, double z)
{
	position = Vec3(x,y,z);
}

// Get the velocity of this object.
Vec3 Object::getVel()
{
	return velocity;
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
bool Object::checkResidence()
{
	if (position.x() < leaf->min.x() || leaf->max.x() < position.x() ||
		position.y() < leaf->min.y() || leaf->max.y() < position.y() ||
		position.z() < leaf->min.z() || leaf->max.z() < position.z())
	{
		leaf->remove(this);
		env->add(this);
		return false;
	}

	return true;
}

// Sets the leaf that this object *thinks* it's in.
void Object::setResidence(Leaf *l)
{
	leaf = l;
}

// Asks the object "Whick leaf node do you think you're in?"
Leaf *Object::getResidence()
{
	return leaf;
}

// Determines if the passed object collides with this one.
bool Object::collidesWith(Object *o)
{
	Vec3 diff(o->position - position);
	double distance2 = diff * diff;
	double sum = o->radius + radius;
	return (distance2 < sum * sum);
}

