#include <GL/glut.h>
//#include "GLTexture.h"
#include "Model_3DS.h"
#include <cmath>
#ifdef WIN32
	#include <process.h>
#endif
#include <iostream>
#include "ship.h"


#ifndef M_PI
	#define M_PI 3.14159265358979
#endif

// Makes a new, boring ship that just sits there.
Ship::Ship() : Object(),
	direction(0,0,1), degpyr(0,0,0), radpyr(0,0,0),
	roa(0.005), rod(0.001), ros(0.95), rot(0.02)
{
	model.Load("art/felix.3DS"); // Load the model
	for(int i = 0; i < model.numObjects; i++)
		model.Objects[i].rot.y = 0.0f;

//	model.shownormals = false;

//	if (!file)
//	{
//		std::cerr << "***ERROR*** Loading 3DS file failed." << std::endl;
//		exit(1);
//	}
//	else
//	{
//		std::cout << "3DS file loaded successfully!" << std::endl;
//	}
}

// Deconstructor: file is a pointer, so delete it
Ship::~Ship() {}

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
	glRotated(degpyr.x(),  1,0,0);
	glRotated(degpyr.z(),  0,0,1);

	#ifndef WIN32
		// Set color to purple.
		glColor3f(.7,0,.8);
	#endif

	// Drawing function
 	// You can also build a texture with a single color and use it
	//GLTexture tex3;
	//tex3.BuildColorTexture(255, 0, 0);	// Builds a solid red texture
	//tex3.Use();				 // Binds the targa for use
	model.Draw();			// Renders the model to the screen

	glPopMatrix();
}

void Ship::recompdir()
{
	direction = Vec3(sin(radpyr.y()) * cos(radpyr.x()), -sin(radpyr.x()), cos(radpyr.y()) * cos(radpyr.x()));
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
	velocity -= direction * rod;
}

// Brings the velocity down to 0
void Ship::stabilize()
{
	if (velocity.norm() <= 3 * roa)
		velocity = Vec3(0,0,0);
	else
		velocity *= ros;
}

// Tilt the nose up.
void Ship::pitchBack()
{
	radpyr -= Vec3(rot, 0, 0);
	recompdir();
}

// Tilt the nose down.
void Ship::pitchForward()
{
	radpyr += Vec3(rot, 0, 0);
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

