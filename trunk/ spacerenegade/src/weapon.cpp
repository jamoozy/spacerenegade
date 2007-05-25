#include <iostream>
#include "GL/glut.h"
#include "environment.h"
#include "vec3.h"
#include "weapon.h"
#include "object.h"
#include "ship.h"


const double Weapon::WEAPON_SPEED = 1.4;

Weapon::Weapon(Ship *shooter, unsigned int ttl) :
	Object(shooter->getPos(), shooter->getVel() + shooter->getDir() * Weapon::WEAPON_SPEED),
	shooter(shooter), ttl(ttl), killNextTick(false) {}

Weapon::~Weapon() {}

void Weapon::draw()
{
	position += velocity;

	glPushMatrix();

	glTranslated(position.x(), position.y(), position.z());

	glColor3d(0.8,0,0);
	glutSolidCube(.5);

	glPopMatrix();

	if (killNextTick || ttl-- == 0) selfDistruct();
}

void Weapon::hits(Object *o)
{
	if (o != shooter)
	{
		std::cout << "Bullet hit!" << std::endl;
		killNextTick = true;
	}
}

void Weapon::selfDistruct()
{
	getResidence()->remove(this);
	delete this;
}

