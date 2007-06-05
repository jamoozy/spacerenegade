#include <iostream>
#include <cmath>
#include "GL/glut.h"
#include "display.h"
#include "environment.h"
#include "buildcomponent.h"
#include "planet.h"

using std::cout;
using std::cerr;
using std::endl;

extern OctTree *env;

// Creates a new planet at the given point
Planet::Planet(double radius, const Vec3& pos) :
	Object("art/felix.3DS", pos, Vec3()) // temporary model for now, add textures later?
{ setRadius(radius); }

// Scale this down.  Allows for smaller planets.
void Planet::setRadius(double r)
{
	model.scale = r/5;
	radius = r;
}

// Draw this planet.
void Planet::draw()
{
	glPushMatrix();

	//glColor3f(0.0f,1.0f,0.0f);
	glTranslated(position.x(), position.y(), position.z());
	
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

void Planet::hits(Object *o)
{
	Object::hits(o);
}

