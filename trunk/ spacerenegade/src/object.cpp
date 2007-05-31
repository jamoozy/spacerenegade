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

// Create a new obect at the origin that does not move.
Object::Object(char *modelName) : leaf(NULL), red(1), green(1), blue(1), radius(4.0), position(0,0,0), velocity(0,0,0), damage(0)
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
Object::Object(char *modelName, const Vec3& pos, const Vec3& v) :
	leaf(NULL), red(1), green(1), blue(1), radius(4.0), position(pos), velocity(v), damage(0)
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
	GLdouble m[16];
	
	Vec3 lclPos = position - playerShip->getPos();
	Vec3 cameraDir = Camera::getCamera()->getDir();
	cameraDir.normalize();
	//double rho = lclPos.norm();
	//double pantaloons = atan(playerShip->getDir().y()/playerShip->getDir().x());
	GLdouble theta = 0;
	GLdouble phi = 0;
	if(cameraDir.x() != 0)
	//if(playerShip->getDir().x() != 0)
		theta = 180 * atan(cameraDir.y()/cameraDir.x()) / M_PI;
	if(cameraDir.norm() != 0)
		phi = 180 * acos(cameraDir.z()/cameraDir.norm()) / M_PI;

	// Use OpenGL to compute a Matrix and get it.
	glPushMatrix();
	glLoadIdentity();
	glRotated(phi,0,0,1);
	glRotated(theta,0,1,0);
	glGetDoublev(GL_MODELVIEW_MATRIX, m);
	Vec3 miniMapPos = m*lclPos;
	glPopMatrix();

	double zBright = (miniMapPos.z() + r) / (2*r);
	double xShift = -85*((miniMapPos.x()) / r);
	double yShift = 85*((miniMapPos.y()) / r);

	glColor3d(red*zBright, green*zBright, blue*zBright);
	glCircle(miniMapX + xShift, miniMapY + yShift, 2, 5);
	glPopMatrix();
}

void Object::hits(Object *o)
{
	// TODO: Put PM's collision code here.
}

