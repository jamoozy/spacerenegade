#ifdef WIN32
	#include <process.h>
#endif
#include <iostream>
#include <GL/glut.h>
#include <cmath>
#include "Model_3DS.h"
#include "environment.h"
#include "camera.h"
#include "ship.h"
#include "planet.h"
#include "display.h"

using std::cout;
using std::cerr;
using std::endl;

extern OctTree *env;

#ifndef M_PI
	#define M_PI 3.14159265358979
#endif

class GLUquadric {};
GLvoid glDrawCube();

////////////////////////////////////////////////////////////////////////////////
// ----------------------- General-Purpose Ship ----------------------------- //
////////////////////////////////////////////////////////////////////////////////

Ship::Ship(char* modelName, Weapon *weapon, Hull *hull, Shield *shield, double fuel) :
	Object(modelName,1,1,1), weapon(weapon), hull(hull), shield(shield),
	tractorBeam(new BasicTractorBeam()), fuel(fuel)
{
	// This big messy thing is the initialization of pitchF et al.
	pitchF[0] = pitchF[15] = 1;
	pitchF[1] = pitchF[2] = pitchF[3] = pitchF[4] = pitchF[7] = pitchF[8] =
		pitchF[11] = pitchF[12] = pitchF[13] = pitchF[14] = 0;
	pitchF[5] = pitchF[10] = cos(rot());
	pitchF[9] = -(pitchF[6] = sin(rot()));

	for (int i = 0; i < 16; i++)
		pitchB[i] = pitchF[i];
	pitchB[6] = -pitchB[6];
	pitchB[9] = -pitchB[9];

	// Yaw left and right.
	yawL[1] = yawL[3] = yawL[4] = yawL[6] = yawL[7] = yawL[9] =
		yawL[11] = yawL[12] = yawL[13] = yawL[14] = 0;
	yawL[5] = yawL[15] = 1;
	yawL[0] = yawL[10] = cos(rot());
	yawL[2] = -(yawL[8] = sin(rot()));

	for (int i = 0; i < 16; i++)
		yawR[i] = yawL[i];
	yawR[8] = -yawR[8];
	yawR[2] = -yawR[2];

	// Roll left and right.
	rollL[2] = rollL[3] = rollL[6] = rollL[7] = rollL[8] = rollL[9] =
		rollL[11] = rollL[12] = rollL[13] = rollL[14] = 0;
	rollL[10] = rollL[15] = 1;
	rollL[0] = rollL[5] = cos(rot());
	rollL[1] = -(rollL[4] = sin(rot()));

	for (int i = 0; i < 16; i++)
		rollR[i] = rollL[i];
	rollR[1] = -rollR[1];
	rollR[4] = -rollR[4];

	// Initialize the local coordinate system to identity.
	GLdouble m[] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
	lcs = m;
}

void Ship::update()
{
	position += velocity;
	shield->update();
}

// This shouldn't ever be called.  But just in case ...
void Ship::draw(int pass)
{
	glPushMatrix();

	//move
	glTranslated(position.x(), position.y(), position.z());
	
	// direction rotation
	glMultMatrixd(lcs.array());

	if (pass == 1)
	{
		// Set color to purple.
		glColor3f(.7,0,.8);
		glutWireCone(2,4,5,1);
	}
	else
	{
		// Show the neat shield-bubble.
		shield->draw();
		tractorBeam->draw(this);
	}
	glPopMatrix();
}

void Ship::hurt(double amt)
{
	hull->hurt(shield->hurt(amt));
}

void Ship::stabilize()
{
	if (velocity.norm() <= 3 * roa())
		velocity = Vec3(0,0,0);
	else
		velocity *= ros();
}



////////////////////////////////////////////////////////////////////////////////
// ------------------------- Player's Ship ---------------------------------- //
////////////////////////////////////////////////////////////////////////////////

// Makes a new, boring ship that just sits there.
PShip::PShip(Weapon *weapon, Hull *hull, Shield *shield) :
	Ship("./art/personalship.3DS", weapon, hull, shield, maxFuel())
{
	// Load variables 
	skymapLoaded = skymap.Load("./art/sky.bmp");

	// Load the sound.
	if ((oinkBuffer = alutCreateBufferFromFile("art/oink.wav")) == AL_NONE) {
		cout << "No buffer returned!" << endl;
		cout << alutGetErrorString(alutGetError()) << endl;
	} else {
		alGenSources(1, &oinkSource);
		alSourcei(oinkSource, AL_BUFFER, oinkBuffer);
	}
}

// Draws the ship.
void PShip::draw(int pass)
{
	glPushMatrix();

	glTranslated(position.x(), position.y(), position.z());

	// Render the skymap
	if (pass == 1 && skymapLoaded) 
	{
		glPushMatrix();

		skymap.Use();
		glScaled(2000, 2000, 2000);
		glDrawCube();

		glPopMatrix();
	}

	// direction rotation
	glMultMatrixd(lcs.array());

	if (pass == 1)
	{
		if (modelLoaded)
		{
			drawReticle();
			glColor3d(1,1,1);
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
	}
	else
	{
		// Draw that neat shield-bubble around the ship.
		shield->draw();
		tractorBeam->draw(this);
	}

	glPopMatrix();
}

void PShip::drawReticle()
{
	glDisable(GL_TEXTURE_2D);
	glColor3f(0,1,0);
	glBegin(GL_LINE_STRIP);
	glVertex3f(-2,-2,50);
	glVertex3f( 2,-2,50);
	glVertex3f( 2, 2,50);
	glVertex3f(-2, 2,50);
	glVertex3f(-2,-2,50);
	glEnd();
//	glBegin(GL_LINE_STRIP);
//	glVertex3f(-2,-2,40);
//	glVertex3f( 2,-2,40);
//	glVertex3f( 2, 2,40);
//	glVertex3f(-2, 2,40);
//	glVertex3f(-2,-2,40);
//	glEnd();

	glBegin(GL_LINES);
	glVertex3f( 0, 3,50);
	glVertex3f( 0,-3,50);
	glVertex3f(-3, 0,50);
	glVertex3f( 3, 0,50);
//	glVertex3f( 0, 3,40);
//	glVertex3f( 0,-3,40);
//	glVertex3f(-3, 0,40);
//	glVertex3f( 3, 0,40);
	glEnd();
}

void PShip::fire()
{
	if (oinkBuffer != AL_NONE)
		alSourcePlay(oinkSource);

	weapon->fire(this);
}

void PShip::hits(Object *o)
{
	Object::hits(o);

	// This is a constant right now.  Later it will be a function
	// of different things like hull strength and shields.
	if (o->shouldHurt(this))
		hurt(200);
	else if (o->canLandOn())
	{
		Planet *p = dynamic_cast<Planet*> (o);
		landOn(p);
	}
}

// Adds to the ship's velocity.
void PShip::accelerate()
{
	if (fuel - 1 >= 0)
	{
		--fuel;
		Ship::accelerate();
	}
}

// Subtracts from the ship's velocity.
void PShip::decelerate()
{
	if (fuel - 1 >= 0)
	{
		--fuel;
		Ship::decelerate();
	}
}

// Brings the velocity down to 0
void PShip::stabilize()
{
	if (fuel - 5 >= 0)
	{
		fuel -= 5;
		Ship::stabilize();
	}
}

// Tilt the nose up.
void PShip::pitchBack()
{
	if (fuel - 0.2 >= 0)
	{
		fuel -= 0.2;
		Ship::pitchBack();
	}
}

// Tilt the nose down.
void PShip::pitchForward()
{
	if (fuel - 0.2 >= 0)
	{
		fuel -= 0.2;
		Ship::pitchForward();
	}
}

// Turn left about the Y(up)-axis
void PShip::yawLeft()
{
	if (fuel - 0.2 >= 0)
	{
		fuel -= 0.2;
		Ship::yawLeft();
	}
}

// Turn right about the Y(up)-axis
void PShip::yawRight()
{
	if (fuel - 0.2 >= 0)
	{
		fuel -= 0.2;
		Ship::yawRight();
	}
}

// Roll to the left - changes orientation.
void PShip::rollLeft()
{
	if (fuel - 0.2 >= 0)
	{
		fuel -= 0.2;
		Ship::rollLeft();
	}
}

// Roll to the right - changes orientation.
void PShip::rollRight()
{
	if (fuel - 0.2 >= 0)
	{
		fuel -= 0.2;
		Ship::rollRight();
	}
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

void PShip::landOn (Planet *planet)
{
	initPlanet();
}
