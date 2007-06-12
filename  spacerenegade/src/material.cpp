#include <iostream>
#include "GL/glut.h"
#include "ship.h"
#include "material.h"

using std::cout;
using std::cerr;
using std::endl;

extern PShip *playerShip;


void Material::update()
{
	position += velocity;

	if (killNextTick) delete this;
}

void Material::draw(int pass)
{
	if (pass == 1)
	{
		glDisable(GL_TEXTURE_2D);
		glPushMatrix();

		glColor3f(1,1,1);
		glTranslated(position.x(), position.y(), position.z());
		glutSolidSphere(1,10,4);

		glPopMatrix();
	}
}

void Material::hits(Object *o)
{
	if (o == playerShip)
	{
		// Add in the "you get this resource" part.
	}
	
	killNextTick = true;
}

