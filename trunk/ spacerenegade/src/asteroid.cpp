#include <iostream>
#include <cmath>
#include "GL/glut.h"
#include "display.h"
#include "environment.h"
#include "buildcomponent.h"
#include "asteroid.h"

using std::cout;
using std::cerr;
using std::endl;

extern OctTree *env;

// Creates a new stationary Asteroid at the origin.
Asteroid::Asteroid(double radius) : Object("art/asteroid.3DS",1,1,1),
	angle(rand()/(3.0 * RAND_MAX),rand()/(3.0 * RAND_MAX),0),
	avelocity(rand()/(3.0 * RAND_MAX),rand()/(3.0 * RAND_MAX),0)
{ setRadius(radius); }

// Creates a new asteroid at the given point with the given velocity.
Asteroid::Asteroid(double radius, double px, double py, double pz, double vx, double vy, double vz) :
	Object("art/asteroid.3DS", 255, 255, 255, px, py, pz, vx, vy, vz),
	angle(rand()/(3.0 * RAND_MAX),rand()/(3.0 * RAND_MAX),0),
	avelocity(rand()/(3.0 * RAND_MAX),rand()/(3.0 * RAND_MAX),0)
{ setRadius(radius); }

// Creates a new asteroid at the given point with the given velocity.
Asteroid::Asteroid(double radius, const Vec3& pos, const Vec3& v) :
	Object("art/asteroid.3DS", pos, v,1,1,1),
	angle(rand()/(3.0 * RAND_MAX),rand()/(3.0 * RAND_MAX),0),
	avelocity(rand()/(3.0 * RAND_MAX),rand()/(3.0 * RAND_MAX),0)
{ setRadius(radius); }

// Scale this down.  Allows for smaller asteroids.
void Asteroid::setRadius(double r)
{
	model.scale = r/5;
	radius = r;
}

// Returns a random position somewhere within this asteroid.
Vec3 Asteroid::randPos() const
{
	return Vec3(position.x() + ((2.0*rand()/RAND_MAX)-1)*radius,
				position.y() + ((2.0*rand()/RAND_MAX)-1)*radius,
				position.z() + ((2.0*rand()/RAND_MAX)-1)*radius);
}

// Update the asteroid's internal state.
void Asteroid::update()
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

	// Destroy myself if I have no health left.
	if (damage >= maxHealth())
	{
		soundFactory->play("explosion-asteroid");
		// If this is still big enough, add small asteroids.
		if (radius > 3)
		{
			Asteroid *a;
			for (int i = 0; i < 4; i++)
			{
				Vec3 pos = randPos();
				Vec3 vel(rr(0.1,-0.1), rr(0.1,-0.1), rr(0.1,-0.1));
				a = new Asteroid(radius/3, pos, vel);
				env->add(a);
			}
		}
		else
		{
			Material *m = new Material(position, velocity);
			env->add(m);
		}
		delete this;
	}
}

// Draw this asteroid.
void Asteroid::draw(int pass)
{
	if (pass == 1)
	{
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
	}
}

void Asteroid::hits(Object *o)
{
	Object::hits(o);
}

