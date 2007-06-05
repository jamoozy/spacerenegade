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
	shooter(shooter), ticks(0), killNextTick(false) { radius = .2; }

void Ammo::hits(Object *o)
{
	if (o != shooter)
	{
		o->hurt(damage());
		killNextTick = true;
	}
};



////////////////////////////////////////////////////////////////////////////////
// --------------------------------- Bullet --------------------------------- //
////////////////////////////////////////////////////////////////////////////////

Bullet::Bullet(Ship *shooter) : Ammo(shooter)
{
	velocity = shooter->getVel() + shooter->getDir() * speed();
};

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
// ------------------------------- Laser Beam ------------------------------- //
////////////////////////////////////////////////////////////////////////////////

LaserBeam::LaserBeam(Ship *shooter) : Ammo(shooter)
{
	velocity = shooter->getVel() + shooter->getDir() * speed();

	B = 0.57735f * shooter->getUp() + 5 * shooter->getDir();
	C = -0.5f * shooter->getLeft() + -0.42265f * shooter->getUp() + 5 * shooter->getDir();
	D = 0.5f * shooter->getLeft() + -0.42265f * shooter->getUp() + 5 * shooter->getDir();
	E = 10 * shooter->getDir();
};

void LaserBeam::draw()
{
	position += velocity;

	glPushMatrix();

	glTranslated(position.x(), position.y(), position.z());
	glDisable(GL_TEXTURE_2D); // Gives full color, for some reason.
	glColor3d(0.6,1.0,0.6);

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3dv(B.array());
	glVertex3dv(C.array());
	glVertex3dv(A.array());
	glVertex3dv(D.array());
	glVertex3dv(B.array());
	glVertex3dv(E.array());
	glVertex3dv(C.array());
	glVertex3dv(D.array());
	glEnd();

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



////////////////////////////////////////////////////////////////////////////////
// -------------------------------- Laser ----------------------------------- //
////////////////////////////////////////////////////////////////////////////////


void Laser::fire(Ship *shooter)
{
	--ammo;

	// This deletes itself after 120 frames (~3s) or it hits something.
	LaserBeam *b = new LaserBeam(shooter);
	env->add(b);
}

////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Tractor Beams ---------------------------- //
////////////////////////////////////////////////////////////////////////////////

void BasicTractorBeam::draw(Ship *ship)
{
	if (on)
	{
		Vec3 a = ship->getLeft() - 2.4142 * ship->getUp();
		Vec3 b = 2.4142 * ship->getLeft() - ship->getUp();
		Vec3 c = 2.4142 * ship->getLeft() + ship->getUp();
		Vec3 d = ship->getLeft() + 2.4142 * ship->getUp();
		Vec3 e = -a;
		Vec3 f = -b;
		Vec3 g = -c;
		Vec3 h = -d;

		Vec3 A = range() * a / 3 + range() * ship->getDir();
		Vec3 B = range() * b / 3 + range() * ship->getDir();
		Vec3 C = range() * c / 3 + range() * ship->getDir();
		Vec3 D = range() * d / 3 + range() * ship->getDir();
		Vec3 E = range() * e / 3 + range() * ship->getDir();
		Vec3 F = range() * f / 3 + range() * ship->getDir();
		Vec3 G = range() * g / 3 + range() * ship->getDir();
		Vec3 H = range() * h / 3 + range() * ship->getDir();

		glDisable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glColor4d(1,0,0,0.5);
		glBegin(GL_QUAD_STRIP);
		glVertex3dv(a.array());
		glVertex3dv(A.array());
		glVertex3dv(b.array());
		glVertex3dv(B.array());
		glVertex3dv(c.array());
		glVertex3dv(C.array());
		glVertex3dv(d.array());
		glVertex3dv(D.array());
		glVertex3dv(e.array());
		glVertex3dv(E.array());
		glVertex3dv(f.array());
		glVertex3dv(F.array());
		glVertex3dv(g.array());
		glVertex3dv(G.array());
		glVertex3dv(h.array());
		glVertex3dv(H.array());
		glVertex3dv(a.array());
		glVertex3dv(A.array());
		glEnd();
		glDisable(GL_BLEND);
	}
}

