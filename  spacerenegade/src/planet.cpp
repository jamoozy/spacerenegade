#include <iostream>
#include <cmath>
#include "GL/glut.h"
#include "display.h"
#include "environment.h"
#include "material.h"
#include "planet.h"

using std::cout;
using std::cerr;
using std::endl;

extern OctTree *env;

// Creates a new planet at the given point
Planet::Planet(const Vec3& pos, double radius) :
	Object("art/spacestation.3DS", pos, Vec3(),0,1,0) // temporary model for now, add textures later?
{ setRadius(radius); }

// Draw this planet.
void Planet::draw(int pass)
{
	if (pass == 1)
	{
		glPushMatrix();

		//glColor3f(0.0f,1.0f,0.0f);
		glTranslated(position.x(), position.y(), position.z());
		
		if (modelLoaded)
		{
			glColor3f(1,1,1);
			model.Draw();
		}
		else
		{
			glColor3f(0.6,0.6,0.9);
			glutSolidSphere(radius,16,8);
		} 

		glPopMatrix();
	}
}

void Planet::hits(Object *o)
{
	Object::hits(o);
}

