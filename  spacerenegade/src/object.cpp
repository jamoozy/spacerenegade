#include <iostream>
#include <cmath>
#include <GL/glut.h>
#include "object.h"
#include "vec3.h"
#include "ship.h"
#include "environment.h"
#include "display.h"
#include "camera.h"

#ifndef M_PI
	#define M_PI 3.14159265358979
#endif

using namespace std;

extern PShip *playerShip;
extern OctTree *env;
extern GLfloat miniMapX;
extern GLfloat miniMapY;

// Create a new obect at the origin that does not move with white for the minimap color
Object::Object(char *modelName) : leaf(NULL), red(1), green(1), blue(1),
	radius(4.0), position(0,0,0), velocity(0,0,0), damage(0)
{
	if (modelName != NULL && modelName[0] != '\0')
		modelLoaded = model.Load(modelName);
}

// Create a new obect at the origin that does not move.
Object::Object(char *modelName, double red, double green, double blue) : leaf(NULL), red(red), green(green), blue(blue),
	radius(4.0), position(0,0,0), velocity(0,0,0), damage(0)
{
	if (modelName != NULL && modelName[0] != '\0')
		modelLoaded = model.Load(modelName);
}

// Create a new obect at the origin that does not move with the specified radius with white for the minimap color
Object::Object(char *modelName, double objRadius) : leaf(NULL), red(1), green(1), blue(1), radius(objRadius), position(0,0,0), velocity(0,0,0), damage(0)//, faction(Attitude::NEUTRAL)
{
	if (modelName != NULL && modelName[0] != '\0')
		modelLoaded = model.Load(modelName);
}

// Create a new obect at the origin that does not move with the specified radius with white for the minimap color
Object::Object(char *modelName, double objRadius, double red, double green, double blue) : 
	leaf(NULL), red(red), green(green), blue(blue), radius(objRadius), position(0,0,0), velocity(0,0,0), damage(0)//, faction(Attitude::NEUTRAL)
{
	if (modelName != NULL && modelName[0] != '\0')
		modelLoaded = model.Load(modelName);
}

// Creates a new object at the given point with the given velocity.
Object::Object(char *modelName, double red, double green, double blue, double px, double py, double pz, double vx, double vy, double vz) :
	leaf(NULL), red(red), green(green), blue(blue), radius(4.0), position(px,py,pz), velocity(vx,vy,vz), damage(0)
{
	if (modelName != NULL && modelName[0] != '\0')
		modelLoaded = model.Load(modelName);
}

// Creates a new object at the given point with the given velocity.
Object::Object(char *modelName, const Vec3& pos, const Vec3& v, double red, double green, double blue) :
	leaf(NULL), red(red), green(green), blue(blue), radius(4.0), position(pos), velocity(v), damage(0)
{
	if (modelName != NULL && modelName[0] != '\0')
		modelLoaded = model.Load(modelName);
}

// Does nothing (nothing to be done).
Object::~Object()
{
	if (leaf != NULL) {
		leaf->remove(this);
		leaf = NULL;
	}
}

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

void Object::drawOnMiniMap(double r)
{
	Vec3 lclPos = position - playerShip->getPos();

	Vec3 normal = Camera::getCamera()->getDir();
	Vec3 up = Camera::getCamera()->getUp();
	Vec3 right = normal ^ up;
	up = right ^ normal;
	normal.normalize();
	up.normalize();
	right.normalize();

	// Find the projection of the object's position on the plane
	// perpendicular to the player that intersects the player.
	double z = -(lclPos * normal);
	double y = (85/r) * (lclPos * up);
	double x = (85/r) * (lclPos * right);
	double alpha = 0;

	double zBrightness = 1 - sqrt((x*x) + (y*y) + (z*z))/r;//1 - ((z + r) / (2 * r));
	if(z <= 0)
		alpha = 1;
	else
		alpha = 0;

	glColor4d(red * zBrightness, green * zBrightness, blue * zBrightness,alpha);
	glCircle(miniMapX + x, miniMapY + y, 2, 5);
}

void Object::hits(Object *o)
{
	// Find the unit vector between the 2 objects.
	Vec3 diff = o->getPos() - position;
	diff.normalize();

	// Find the "speed" that this object hit the other by using
	// the velocity vector and dot product.
	double s = diff * velocity;

	// This means this one is travelling away from the other one
	// OR
	// The other one is travelling away faster than this one is
	// chasing it (happens often when shooting).
	if (s < 0 || s < o->getVel() * diff) return;

	// That should map directly to the force vector...
	Vec3 force = s * diff;
	velocity -= force;
	o->push(force);
}

