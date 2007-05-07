#include <GL/gl.h>
#include <GL/glut.h>
#include <cmath>
#include "ship.h"


// Makes a new, boring ship that just sits there.
Ship::Ship() : Object::Object(), direction(0,0,1), degpyr(0,0,0), radpyr(0,0,0), roa(0.01), ros(0.99), rot(.1) {}

// Set the ship somewhere new.  This should be used primarily
// for debugging purposes.
void Ship::setAt(double x, double y, double z)
{
	position = Vec3(x, y, z);
}

// Draws the ship.
void Ship::draw()
{
	position += velocity;
	
	glPushMatrix();

	//move
	glTranslated(position.x(), position.y(), position.z());
	
	// direction rotation
	glRotated(degpyr.y(),  0,1,0);
	glRotated(degpyr.z(),  0,0,1);
	glRotated(degpyr.x(),  1,0,0);

	// Drawing function
	glutWireCone(-2.5, 5, 4, 2);

	glPopMatrix();
}

void Ship::recompdir()
{
	direction = Vec3(sin(radpyr.y()), sin(radpyr.x()), cos(radpyr.y()) * cos(radpyr.x()));
	degpyr = radpyr * (180 / M_PI);
}

// Adds to the ship's velocity.
void Ship::accelerate()
{
	velocity += direction * roa;
}

// Subtracts from the ship's velocity.
void Ship::decelerate()
{
	velocity -= direction * roa;
}

// Brings the velocity down to 0
void Ship::stabilize()
{
	if (velocity.norm() <= 3 * roa)
		velocity = Vec3(0,0,0);

	velocity *= ros;
}

// Tilt the nose up.
void Ship::pitchBack()
{
	radpyr += Vec3(rot, 0, 0);
	recompdir();
}

// Tilt the nose down.
void Ship::pitchForward()
{
	radpyr -= Vec3(rot, 0, 0);
	recompdir();
}

// Turn left about the Y(up)-axis
void Ship::yawLeft()
{
	radpyr += Vec3(0, rot, 0);
	recompdir();
}
	
// Turn right about the Y(up)-axis
void Ship::yawRight()
{
	radpyr -= Vec3(0, rot, 0);
	recompdir();
}

// 
void Ship::rollLeft()
{
//	radpyr += Vec3(0, 0, rot);
//	recompdir();
}

// 
void Ship::rollRight()
{
//	radpyr -= Vec3(0, 0, rot);
//	recompdir();
}

