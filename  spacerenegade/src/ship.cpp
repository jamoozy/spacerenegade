#include <GL/glut.h>
//#include "GLTexture.h"
#include "Model_3DS.h"
#include <cmath>
#ifdef WIN32
	#include <process.h>
#endif
#include <iostream>
#include "environment.h"
#include "weapon.h"
#include "ship.h"

extern OctTree *env;

#ifndef M_PI
	#define M_PI 3.14159265358979
#endif

// Makes a new, boring ship that just sits there.
Ship::Ship() : Object("./art/personalship.3DS"),
	direction(0,0,1), degpyr(0,0,0), radpyr(0,0,0), //lightRnotL(true), lightTime(0),
	roa(0.005), rod(0.001), ros(0.95), rot(0.02) {}

// Deconstructor: file is a pointer, so delete it
Ship::~Ship() {}

// Draws the ship.
void Ship::draw()
{
	position += velocity;

	glPushMatrix();

	//move
	glTranslated(position.x(), position.y(), position.z());
	
	// direction rotation
	glRotated(degpyr.z(),  0,0,1);
	glRotated(degpyr.y(),  0,1,0);
	glRotated(degpyr.x(),  1,0,0);

	if (modelLoaded)
	{
		glColor3d(1,1,1);
		// Drawing function
		// You can also build a texture with a single color and use it
		//GLTexture tex3;
		//tex3.BuildColorTexture(255, 0, 0);  // Builds a solid red texture
		//tex3.Use();  // Binds the targa for use
		model.Draw();  // Renders the model to the screen

		//GLfloat lightColor[] = {0.0f,0.0f,0.0f};
		//GLfloat pos[3] = {3.0f,1.0f,-1.5f};

		//if (lightRnotL)
		//{
		//	lightColor[2] = 1.0f;
		//	pos[0] = -3.0f;
		//}
		//else
		//{
		//	lightColor[0] = 1.0f;
		//}
		//if (lightTime++ > 40) { lightRnotL = !lightRnotL; lightTime = 0; }

		//glColor3fv(lightColor);
		//glLightfv(GL_LIGHT1, GL_POSITION, pos);
		//glLightfv(GL_LIGHT1, GL_AMBIENT,  lightColor);
		//glLightfv(GL_LIGHT1, GL_DIFFUSE,  lightColor);
		//glLightfv(GL_LIGHT1, GL_SPECULAR, lightColor);
		//glTranslatef(pos[0],pos[1],pos[2]);
		//glutSolidCube(0.2);
	}
	else
	{
		// Set color to purple.
		glColor3f(.7,0,.8);
		glutWireCone(2,4,5,1);
	}

	glPopMatrix();
}

void Ship::hits(Object *o)
{
//	// TODO: damage the ship?
//	std::cout << "Ship collision!" << std::endl;

	// This is a constant right now.  Later it will be a function of things,
	// like hull defense, shields, and stuff.
	hurt(200);
}

void Ship::fire()
{
	Weapon *w = new Weapon(this,120);
	env->add(w);
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

