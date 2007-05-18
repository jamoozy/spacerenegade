#include "GL/glut.h"
#include "vec3.h"
#include "weapon.h"
#include "object.h"
#include "ship.h"


const double Weapon::WEAPON_SPEED = 30.0;

Weapon::Weapon(Ship *shooter) :
	Object(Vec3(0,0,0), shooter->getDir() * Weapon::WEAPON_SPEED),
	shooter(shooter) {}

Weapon::~Weapon() {}

void Weapon::draw()
{
	position += velocity;

	glPushMatrix();

	glTranslated(position.x(), position.y(), position.z());

	glColor3d(1,0,0);
	glutSolidCube(.2);

	glPopMatrix();
}

bool Weapon::collidesWith(Object *o)
{
	return (Object::collidesWith(o));
}
