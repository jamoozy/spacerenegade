#include <iostream>
#include <cmath>
#include "GL/glut.h"
#include "asteroid.h"

// Creates a new stationary Asteroid at the origin.
Asteroid::Asteroid() :
	Object(),
	angle(rand()/(3.0 * RAND_MAX),rand()/(3.0 * RAND_MAX),0),
	avelocity(rand()/(3.0 * RAND_MAX),rand()/(3.0 * RAND_MAX),0) {}

// Creates a new asteroid at the given point with the given velocity.
Asteroid::Asteroid(double px, double py, double pz, double vx, double vy, double vz) :
	Object(px, py, pz, vx, vy, vz),
	angle(rand()/(3.0 * RAND_MAX),rand()/(3.0 * RAND_MAX),0),
	avelocity(rand()/(3.0 * RAND_MAX),rand()/(3.0 * RAND_MAX),0) {}

// Creates a new asteroid at the given point with the given velocity.
Asteroid::Asteroid(const Vec3& pos, const Vec3& v) :
	Object(pos, v),
	angle(rand()/(3.0 * RAND_MAX),rand()/(3.0 * RAND_MAX),0),
	avelocity(rand()/(3.0 * RAND_MAX),rand()/(3.0 * RAND_MAX),0) {}

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
