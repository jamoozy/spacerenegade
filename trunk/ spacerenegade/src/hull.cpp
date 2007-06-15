#include <GL/glut.h>
#include "hull.h"


// Changes the state of the shield.
void Shield::update()
{
	if (health + ror() > maxHlth())
		health = maxHlth();
	else
		health += ror();
}

// Draws this shield.  Brightness varies with its 'health'.
void Shield::draw()
{
	// Jam:
	// Eventually it would be best to put a texture here instead
	// of drawing a sphere because textures could be rounder and
	// would be more uniform (view independant).
	glDisable(GL_TEXTURE_2D);
	glColor4d(1,1,0,hlthPcnt()/2);
	glutSolidSphere(5, 32, 8);
}

// Hurts this shield, effecting its 'health'.
// Returns the amount of damage it could not take.
//
// If this returns 0 then the shield absorbed all of the damage and it's
// safe to assume it can absort at least a little more.
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


// Fill up by this much more.
// Return true if this can hold w, false otherwise.
bool CargoBay::add(double w)
{
	if (weight + w > capacity())
		return false;

	weight += w;
	return true;
}
