#include <iostream>
#include "GL/glut.h"
#include "environment.h"
#include "vec3.h"
#include "ship.h"
#include "weapon.h"


extern OctTree *env;


////////////////////////////////////////////////////////////////////////////////
// ---------------------------------- Ammo ---------------------------------- //
////////////////////////////////////////////////////////////////////////////////

Ammo::Ammo(Ship *shooter) :
	Object("", shooter->getPos(), shooter->getVel() + shooter->getDir() * speed()),
	shooter(shooter), ticks(0), killNextTick(false) { radius = .2;}

void Ammo::hits(Object *o)
{
	if (o != shooter)
	{
		o->hurt(damage());
		killNextTick = true;
	}
	else
	{
		// The hacky way not to hurt the shooter of this ammo piece.
		o->hurt(-200);
	}
};



////////////////////////////////////////////////////////////////////////////////
// --------------------------------- Bullet --------------------------------- //
////////////////////////////////////////////////////////////////////////////////

void Bullet::draw()
{
	position += velocity;

	glPushMatrix();

	glTranslated(position.x(), position.y(), position.z());

	glColor3d(0.8,0,0);
	glutSolidCube(.5);

	glPopMatrix();

	if (killNextTick || ticks++ >= ttl()) delete this;
}



////////////////////////////////////////////////////////////////////////////////
// ------------------------------ Blaster ----------------------------------- //
////////////////////////////////////////////////////////////////////////////////

void Blaster::fire(Ship *shooter)
{
	--ammo;

	// This deletes itself after 120 frames (~3s) or it hits something.
	Bullet *b = new Bullet(shooter);
	env->add(b);
}

