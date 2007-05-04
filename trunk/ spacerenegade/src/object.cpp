#include "object.h"


// Create a new obect at the origin that does not move.
Object::Object() : position(0,0,0), velocity(0,0,0) {}

// Creates a new object at the given point with the given velocity.
Object::Object(double px, double py, double pz, double vx, double vy, double vz) :
	position(px,py,pz), velocity(vx,vy,vz) {}

// Creates a new object at the given point with the given velocity.
Object::Object(const Vec3& pos, const Vec3& v) : position(pos), velocity(v) {}

// Does nothing (nothing to be done).
Object::~Object() {}

// Updates the position.
void Object::draw()
{
	position += velocity;
}
