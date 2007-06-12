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
	Object("", shooter->getPos(), shooter->getVel() + shooter->getDir() * speed(),1,0,0),
	shooter(shooter), ticks(0), killNextTick(false) { radius = .2; }

void Ammo::update()
{
	position += velocity;

	if (killNextTick || ticks++ >= ttl()) delete this;
}

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

void Bullet::draw(int pass)
{
	// Only draw on the first pass.
	if (pass == 1)
	{
		glPushMatrix();

		glTranslated(position.x(), position.y(), position.z());
		glColor3d(0.8,0,0);
		glutSolidCube(.5);

		glPopMatrix();
	}
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

void LaserBeam::draw(int pass)
{
	// Only draw on the 1st pass, not the 2nd.
	if (pass == 1)
	{
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
	}
}



////////////////////////////////////////////////////////////////////////////////
// ------------------------------ Blaster ----------------------------------- //
////////////////////////////////////////////////////////////////////////////////

void Blaster::fire(Ship *shooter)
{
	--ammo;
	soundFactory->play("gunshot");
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

BasicTractorBeam::BasicTractorBeam() : TractorBeam(),
	a(-1, -2.4142, 0), b(-2.4142, -1, 0), c(-2.4142,  1, 0), d(-1,  2.4142, 0), 
	e( 2,  2.4142, 0), f( 2.4142,  1, 0), g( 2.4142, -1, 0), h( 1, -2.4142, 0),
	A(a * (range()/2) + Vec3(0,0,range())),
	B(b * (range()/2) + Vec3(0,0,range())),
	C(c * (range()/2) + Vec3(0,0,range())),
	D(d * (range()/2) + Vec3(0,0,range())),
	E(e * (range()/2) + Vec3(0,0,range())),
	F(f * (range()/2) + Vec3(0,0,range())),
	G(g * (range()/2) + Vec3(0,0,range())),
	H(h * (range()/2) + Vec3(0,0,range())) {}

void BasicTractorBeam::update(const Ship* s)
{
	if (on)
	{
		Vec3 pos = s->getPos() + (s->getDir() * range());
		vector<Object*> v;
		env->getArea(pos, range(), v);

		for (vector<Object*>::iterator iter = v.begin(); iter != v.end(); iter++)
		{
			if (*iter != s)
			{
				Vec3 diff(s->getPos() - (*iter)->getPos());
				diff.normalize();
				(*iter)->push(diff * strength());
			}
		}
	}
}

void BasicTractorBeam::draw()
{
	if (on)
	{
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

