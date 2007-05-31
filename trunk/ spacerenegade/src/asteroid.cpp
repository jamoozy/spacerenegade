#include <iostream>
#include <cmath>
#include "GL/glut.h"
#include "environment.h"
#include "asteroid.h"

// Creates a new stationary Asteroid at the origin.
Asteroid::Asteroid() : Object("art/asteroid.3DS"),
	angle(rand()/(3.0 * RAND_MAX),rand()/(3.0 * RAND_MAX),0),
	avelocity(rand()/(3.0 * RAND_MAX),rand()/(3.0 * RAND_MAX),0) { radius = 5.0; }

// Creates a new asteroid at the given point with the given velocity.
Asteroid::Asteroid(double px, double py, double pz, double vx, double vy, double vz) :
	Object("art/asteroid.3DS", 255, 255, 255, px, py, pz, vx, vy, vz),
	angle(rand()/(3.0 * RAND_MAX),rand()/(3.0 * RAND_MAX),0),
	avelocity(rand()/(3.0 * RAND_MAX),rand()/(3.0 * RAND_MAX),0) { radius = 5.0; }

// Creates a new asteroid at the given point with the given velocity.
Asteroid::Asteroid(const Vec3& pos, const Vec3& v) : Object("art/asteroid.3DS", pos, v),
	angle(rand()/(3.0 * RAND_MAX),rand()/(3.0 * RAND_MAX),0),
	avelocity(rand()/(3.0 * RAND_MAX),rand()/(3.0 * RAND_MAX),0) { radius = 5.0; }

Asteroid::~Asteroid() {}

// Draw this asteroid.
void Asteroid::draw()
{
	// Make sure this asteroid doesn't travel out of the bounding box
	// of the environment.
	Vec3 nextPos = position + velocity;
	if (nextPos.x() < -OctTree::BOUND || OctTree::BOUND < nextPos.x())
		velocity = Vec3(-velocity.x(), velocity.y(), velocity.z());
	if (nextPos.y() < -OctTree::BOUND || OctTree::BOUND < nextPos.y())
		velocity = Vec3(velocity.x(), -velocity.y(), velocity.z());
	if (nextPos.z() < -OctTree::BOUND || OctTree::BOUND < nextPos.z())
		velocity = Vec3(velocity.x(), velocity.y(), -velocity.z());

	position += velocity;
	angle += avelocity;

	glPushMatrix();

	//glColor3f(0.0f,1.0f,0.0f);
	glTranslated(position.x(), position.y(), position.z());
	glRotated(angle.x(),  1.0,0.0,0.0);
	glRotated(angle.y(),  0.0,1.0,0.0);
	
	if (modelLoaded)
	{
		glColor3f(1,1,1);
		//glutWireSphere(5,4,2);
		model.Draw();
	}
	else
	{
		glColor3f(1,1,1);
		glutWireSphere(5,16,8);
	} 

	glPopMatrix();

	if (damage >= maxHealth()) delete this;
}

void Asteroid::hits(Object *o)
{
	o->hurt(10);
	o->push((o->getPos() - position)/10);
}
