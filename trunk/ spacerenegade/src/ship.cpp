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

class GLUquadric {};
GLvoid glDrawCube();

////////////////////////////////////////////////////////////////////////////////
// ----------------------- General-Purpose Ship ----------------------------- //
////////////////////////////////////////////////////////////////////////////////

Ship::Ship(char* modelName, double fuel, double ammo) : Object(modelName),
	direction(0,0,1), degpyr(0,0,0), radpyr(0,0,0),
	fuel(fuel), ammo(ammo) {}

Ship::~Ship() {}

// This shouldn't ever be called.  But just in case ...
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

	// Set color to purple.
	glColor3f(.7,0,.8);
	glutWireCone(2,4,5,1);

	glPopMatrix();

}

// Do the object thang.
void Ship::hits(Object *o)
{
	Object::hits(o);
}

void Ship::recompdir()
{
	direction = Vec3(sin(radpyr.y()) * cos(radpyr.x()), -sin(radpyr.x()), cos(radpyr.y()) * cos(radpyr.x()));
	degpyr = radpyr * (180 / M_PI);
}

void Ship::stabilize()
{
	if (velocity.norm() <= 3 * roa())
		velocity = Vec3(0,0,0);
	else
		velocity *= ros();
}

void Ship::pitchBack()
{
	radpyr -= Vec3(rot(),0,0);
	recompdir();
}

void Ship::pitchForward()
{
	radpyr += Vec3(rot(),0,0);
	recompdir();
}

void Ship::yawLeft()
{
	radpyr += Vec3(0,rot(),0);
	recompdir();
}

void Ship::yawRight()
{
	radpyr -= Vec3(0,rot(),0);
	recompdir();
}



////////////////////////////////////////////////////////////////////////////////
// ------------------------- Player's Ship ---------------------------------- //
////////////////////////////////////////////////////////////////////////////////

// Makes a new, boring ship that just sits there.
PShip::PShip() : Ship("./art/personalship.3DS", maxFuel(), maxAmmo())
	//direction(0,0,1), degpyr(0,0,0), radpyr(0,0,0),
	//lightRnotL(true), lightTime(0),
	//roa(0.005), rod(0.001), ros(0.95), rot(0.02),
	//fuel(maxFuel()), ammo(maxAmmo()) {}
{
	// Load variables 
	skymapLoaded = true;
	skymap.Load("./art/sky.bmp");
}

// Deconstructor: file is a pointer, so delete it
PShip::~PShip() {}

// Draws the ship.
void PShip::draw()
{
	position += velocity;

	glPushMatrix();

	//move
	glTranslated(position.x(), position.y(), position.z());
	
	// Render the skymap
	if (skymapLoaded) 
	{
		glPushMatrix();
		// Set properties of the material of the sky map
		GLfloat mat_amb_diff[] = { 0.0, 0.0, 0.0, 1.0 };
		GLfloat mat_emission[] = { 1.0, 1.0, 1.0, 1.0 };
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
		skymap.Use();
		// Draw the skymap
		glScaled(2000, 2000, 2000);
		glDrawCube();
		glPopMatrix();
	}

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

void PShip::hits(Object *o)
{
//	// TODO: damage the ship?
//	std::cout << "PShip collision!" << std::endl;

	// This is a constant right now.  Later it will be a function of things,
	// like hull defense, shields, and stuff.
	hurt(200);
}

void PShip::fire()
{
	--ammo;

	// This deletes itself after 120 frames (~3s) or it hits something.
	Weapon *w = new Weapon(this,120);
	env->add(w);
}

// Adds to the ship's velocity.
void PShip::accelerate()
{
	--fuel;
	Ship::accelerate();
}

// Subtracts from the ship's velocity.
void PShip::decelerate()
{
	--fuel;
	Ship::decelerate();
}

// Brings the velocity down to 0
void PShip::stabilize()
{
	fuel -= 5;
	Ship::stabilize();
}

// Tilt the nose up.
void PShip::pitchBack()
{
	fuel -= 0.2;
	Ship::pitchBack();
}

// Tilt the nose down.
void PShip::pitchForward()
{
	fuel -= 0.2;
	Ship::pitchForward();
}

// Turn left about the Y(up)-axis
void PShip::yawLeft()
{
	fuel -= 0.2;
	Ship::yawLeft();
}

// Turn right about the Y(up)-axis
void PShip::yawRight()
{
	fuel -= 0.2;
	Ship::yawRight();
}

//
void PShip::rollLeft()
{
//	fuel -= 0.2;
//	radpyr += Vec3(0, 0, rot);
//	recompdir();
}

//
void PShip::rollRight()
{
//	fuel -= 0.2;
//	radpyr -= Vec3(0, 0, rot);
//	recompdir();
}

// Draws the sky cube for the environment
GLvoid glDrawCube()
{
		glBegin(GL_QUADS);
		// Front Face
		glNormal3f( 0.0f, 0.0f, 0.5f);					
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		// Back Face
		glNormal3f( 0.0f, 0.0f,-0.5f);					
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		// Top Face
		glNormal3f( 0.0f, 0.5f, 0.0f);					
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		// Bottom Face
		glNormal3f( 0.0f,-0.5f, 0.0f);					
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		// Right Face
		glNormal3f( 0.5f, 0.0f, 0.0f);					
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		// Left Face
		glNormal3f(-0.5f, 0.0f, 0.0f);					
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glEnd();
}

