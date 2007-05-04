#include <iostream>
#include <cmath>
#include "GL/glut.h"
#include "asteroid.h"

// Creates a new stationary Asteroid at the origin.
Asteroid::Asteroid() :
	angle(rand()/(3.0 * RAND_MAX),rand()/(3.0 * RAND_MAX),0),
	avelocity(rand()/(3.0 * RAND_MAX),rand()/(3.0 * RAND_MAX),0)
{
	position = Vec3(0.0,0.0,0.0);
	velocity = Vec3(0.0,0.0,0.0);
}

// Creates a new asteroid at the given point with the given velocity.
Asteroid::Asteroid(double px, double py, double pz, double vx, double vy, double vz) :
	angle(rand()/(3.0 * RAND_MAX),rand()/(3.0 * RAND_MAX),0),
	avelocity(rand()/(3.0 * RAND_MAX),rand()/(3.0 * RAND_MAX),0)
{
	position = Vec3(px,py,pz);
	velocity = Vec3(vx,vy,vz);
}

// Creates a new asteroid at the given point with the given velocity.
Asteroid::Asteroid(const Vec3& pos, const Vec3& v) :
	angle(rand()/(3.0 * RAND_MAX),rand()/(3.0 * RAND_MAX),0),
	avelocity(rand()/(3.0 * RAND_MAX),rand()/(3.0 * RAND_MAX),0)
{
	position = pos;
	velocity = v;
}

// Draw this asteroid.
void Asteroid::draw()
{
	position += velocity;
	angle += avelocity;

	glPushMatrix();

	glColor3f(0.0f,1.0f,0.0f);
	glTranslated(position.x(), position.y(), position.z());
	glRotated(angle.x(),  1.0,0.0,0.0);
	glRotated(angle.y(),  0.0,1.0,0.0);
	glutWireSphere(5,4,2);

	glPopMatrix();
}
