#include <GL/glut.h>
#include "hull.h"



void Shield::update()
{
	if (health + ror() > maxHlth())
		health = maxHlth();
	else
		health += ror();
}

void Shield::draw()
{
	glDisable(GL_TEXTURE_2D);
	glColor4d(1,1,0,hlthPcnt()/2);
	glutSolidSphere(5, 32, 8);
}

double Shield::hurt(double amt)
{
	health -= amt * (1-hap());

	// Jam:
	// This is a fairly normal case when the amount of damage
	// being issued is more than the shield can handle.  In
	// this case, we return the value.  It will then be taken
	// by the hull ... as of the time I wrote this comment.
	double rtn = -health;
	health = 0;
	return rtn;

	// Jam:
	// There used to be another case where damage was negative
	// and would heal the ship, but that was taken out with
	// the addition of the "shouldHurt()" function.
}

