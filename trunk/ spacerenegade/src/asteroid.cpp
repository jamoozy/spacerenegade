#include <iostream>
#include "GL/glut.h"
#include "asteroid.h"

// Creates a new stationary Asteroid at the origin.
Asteroid::Asteroid()
{
	position = Vec3(0.0,0.0,0.0);
	velocity = Vec3(0.0,0.0,0.0);
}

// Creates a new asteroid at the given point with the given velocity.
Asteroid::Asteroid(double px, double py, double pz, double vx, double vy, double vz)
{
	position = Vec3(px,py,pz);
	velocity = Vec3(vx,vy,vz);
}

// Creates a new asteroid at the given point with the given velocity.
Asteroid::Asteroid(const Vec3& pos, const Vec3& v)
{
	position = pos;
	velocity = v;
}

// Draw this asteroid.
void Asteroid::draw()
{
	std::cout << "position before: " << position.str() << std::endl;
	std::cout << "velocity: " << velocity.str() << std::endl;
	position += velocity;
	std::cout << "position after: " << position.str() << std::endl;

	glPushMatrix();

	glColor3f(0.0f,1.0f,0.0f);
	glTranslated(position.x(), position.y(), position.z());
	glutWireSphere(5,8,12);

	glPopMatrix();
}
