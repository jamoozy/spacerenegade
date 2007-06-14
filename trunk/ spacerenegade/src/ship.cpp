#ifdef WIN32
	#include <process.h>
#endif
#include <iostream>
#include <GL/glut.h>
#include <cmath>
#include <vector>
#include "Model_3DS.h"
#include "environment.h"
#include "camera.h"
#include "ship.h"
#include "planet.h"
#include "display.h"
#include "factionInfo.h"
#include "mission.h"
#include "objective.h"

#ifndef M_PI
	#define M_PI 3.14159265358979
#endif

using std::cout;
using std::cerr;
using std::endl;
using std::vector;

extern OctTree *env;
extern FactionInfo *playerFactionInfo, *redFactionInfo, *blueFactionInfo,
	*whiteFactionInfo, *otherFactionInfo;
extern vector<Ship*> enemyShips;
extern vector<Mission*> missionsAvailable;
extern vector<Mission*> missionsOn;

#ifndef M_PI
	#define M_PI 3.14159265358979
#endif

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

////////////////////////////////////////////////////////////////////////////////
// ----------------------- General-Purpose Ship ----------------------------- //
////////////////////////////////////////////////////////////////////////////////

Ship::Ship(char* modelName, Weapon *weapon, Hull *hull, CargoBay *bay, Shield *shield,
		double fuel, double red, double green, double blue) :
	Object(modelName,red,green,blue), weapon(weapon), hull(hull), bay(bay),
	shield(shield), tractorBeam(new BasicTractorBeam()), fuel(fuel), ai(NULL)
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

// Calculate the number of radians the ship has rotated, counterclockwise.
// For example, if the up vector of the ship is (-1, 0, 0), this will return
// pi/2.  If it is (0, -1, 0), this will return pi.  If it is (1, 0, 0),
// it will return pi * 3/2
double Ship::getRadiansRotated() const
{
	Vec3 shipUp = getUp();
	Vec3 worldUp(0, 1, 0);

	double angle = shipUp.angleBetween(worldUp);
	double crossProduct = (shipUp ^ worldUp).z();

	if(crossProduct > 0)
		angle += M_PI;
	return angle;
}

void Ship::update()
{
	position += velocity;
	shield->update();
	tractorBeam->update(this);

	if(ai != NULL)
		ai->update();

	if (hull->getHlth() <= 0)
		destroy();
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
		tractorBeam->draw();
	}
	glPopMatrix();
}

void Ship::fire()
{
	soundFactory->play("gunshot");
	weapon->fire(this);
}

void Ship::hurt(double amt)
{
	hull->hurt(shield->hurt(amt));
}

void Ship::setDir(Vec3& newDir)
{
	Vec3 newLeft = getUp() ^ newDir;
	if (newLeft == Vec3(0,0,0))
		newLeft = getLeft();
	Vec3 newUp = newDir ^ newLeft;

	newDir.normalize();
	newUp.normalize();
	newLeft.normalize();

	lcs.set(0,newLeft.x());
	lcs.set(1,newLeft.y());
	lcs.set(2,newLeft.z());

	lcs.set(4,newUp.x());
	lcs.set(5,newUp.y());
	lcs.set(6,newUp.z());

	lcs.set(8,newDir.x());
	lcs.set(9,newDir.y());
	lcs.set(10,newDir.z());
}

void Ship::eliminateOldInertia()
{
	Vec3 currentVelocity = velocity;
	currentVelocity.normalize();
	Vec3 desiredDirection = getDir();
	Vec3 diff = desiredDirection - currentVelocity;
	diff.normalize();
	velocity += diff * roa();
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
	Ship("./art/personalship.3DS", weapon, hull, new SCargoBay(), shield, maxFuel(), 0.0, 1.0, 0.0)
{
	// Load variables
	skymapLoaded = skymap.Load("./art/sky.bmp");

	faction = playerFactionInfo;

	credits = 10000;

	fuelCost = 1;
	hlthCost = 10;
	ammoCost = 5;
}

void PShip::update()
{
	position += velocity;
	shield->update();
	tractorBeam->update(this);
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
		tractorBeam->draw();
	}

	glPopMatrix();
}

void PShip::drawReticle()
{
	glDisable(GL_TEXTURE_2D);  // Give full color brightness.
	glColor3f(0,1,0);

	// The box.
	glBegin(GL_LINE_STRIP);
	glVertex3f(-2,-2,50);
	glVertex3f( 2,-2,50);
	glVertex3f( 2, 2,50);
	glVertex3f(-2, 2,50);
	glVertex3f(-2,-2,50);
	glEnd();

	// The cross.
	glBegin(GL_LINES);
	glVertex3f( 0, 3,50);
	glVertex3f( 0,-3,50);
	glVertex3f(-3, 0,50);
	glVertex3f( 3, 0,50);
	glEnd();
}


void PShip::refuel()
{
	if (canAfford(costToRefuel()))
	{
		credits -= costToRefuel();
		fuel = maxFuel();
	}
	else // only has partial credits
	{
		int num = credits / fuelCost;
		fuel += num;
		credits -= num * fuelCost;
	}
}

void PShip::reload()
{
	if (canAfford(costToReload()))
	{
		credits -= costToReload();
		weapon->reload();
	}
	else // only has partial credits
	{
		int num = credits / ammoCost;
		addToAmmo(num);
		credits -= num * ammoCost;
	}
}
void PShip::heal()
{
	if (canAfford(costToHeal()))
	{
		credits -= costToHeal();
		hull->heal();
	}
	else // only has partial credits
	{
		int num = credits / hlthCost;
		addToHlth(num);
		credits -= num * hlthCost;
	}
}

void PShip::sellMaterials() // (Gum)
{
	credits += getProfit();
	bay->empty();
}

int PShip::getProfit() // (Gum)
{
	int MATPRICE = 10000; // more/less?
	return (bay->getWeight()) * MATPRICE;
}

void PShip::fire()
{
	weapon->fire(this);
}

void PShip::hits(Object *o)
{
	Object::hits(o);

	// Jam:
	// This check allows bullets NOT to hurt the ship they came from.
	if (o->shouldHurt(this))
	{
		// Jam:
		// This is a constant right now.  Later it will be a function
		// of different things like hull strength and shields.
		hurt(200);
	}
}

void PShip::destroy()
{
	soundFactory->play("explosion-ship");
	delete this;
}

// Adds to the ship's velocity.
void PShip::accelerate()
{
	if (fuel - 1 >= 0)
	{
		--fuel;
		soundFactory->play("thrust");
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

////////////////////////////////////////////////////////////////////////////////
// ------------------------- Basic Red Ship --------------------------------- //
////////////////////////////////////////////////////////////////////////////////

// Makes a new, boring ship that just sits there.
BasicRedShip::BasicRedShip(Weapon *weapon, Hull *hull, Shield *shield) :
	Ship("./art/basicredship.3DS", weapon, hull, new CargoBay(), shield, maxFuel(), 1.0, 0.0, 0.0)
{
	ai = new ShipAI(this, redFactionInfo);
	faction = redFactionInfo;
}

BasicRedShip::~BasicRedShip()
{
	vector<Ship*>::iterator startIterator;
	vector<Ship*>::iterator tempIterator;
	for( tempIterator = enemyShips.begin(); tempIterator != enemyShips.end(); tempIterator++ ) {
		if(*tempIterator == this)
		{
			enemyShips.erase(tempIterator);
			//cout << "erased" << endl;
			break;
		}
    }
}

// Draws the ship.
void BasicRedShip::draw(int pass)
{
	glPushMatrix();

	// Draw the current path
/*	glPushMatrix();
	glColor3d(1.0, 0.0, 0.0);
	vector<Vec3> currentPath = ai->getPath();
	glBegin(GL_LINES);
	for(int i = 0; i < static_cast<int>(currentPath.size()) - 1; i++)
	{
		Vec3 start = currentPath.at(i);
		Vec3 end = currentPath.at(i + 1);
		glVertex3d(start.x(), start.y(), start.z());
		glVertex3d(end.x(), end.y(), end.z());
	}
	glEnd();
	glPopMatrix();
*/
	glTranslated(position.x(), position.y(), position.z());

	// direction rotation
	glMultMatrixd(lcs.array());

	if (pass == 1)
	{
		if (modelLoaded)
		{
			glColor3d(1,1,1);
			model.Draw();  // Renders the model to the screen
		}
		else
		{
			// Set color to red.
			glColor3f(1.0, 0.0, 0.0);
			glutWireCone(2,4,5,1);
		}
	}
	else
	{
		// Draw that neat shield-bubble around the ship.
		shield->draw();
		tractorBeam->draw();
	}

	glPopMatrix();
}

void BasicRedShip::hits(Object *o)
{
	Object::hits(o);

	// Jam:
	// This check allows bullets NOT to hurt the ship they came from.
	if (o->shouldHurt(this))
	{
		// Jam:
		// This is a constant right now.  Later it will be a function
		// of different things like hull strength and shields.
		hurt(200);
	}
	//cout << "Enemy ship hp: " << getHlth() << ", Shield: " << shield->getHlth() << endl;
	// Make it turn on player if the object was a bullet
	if(faction->getAttitude(PLAYER) == NEUTRAL && o->aiShouldConsider())
	{
		faction->setAttitude(ENEMY, PLAYER);
	}
}

void BasicRedShip::destroy()
{
	soundFactory->play("explosion-ship");
	//cout << "Destroyed" << endl;
		Mission *m = NULL;
	for(unsigned int i = 0; i < missionsOn.size(); i++)
	{
		m = missionsOn.at(i);
		if(m->getID() == 0)
		{
			m->getObjective(0)->incrementKills();
		}
	}
	delete this;
}



////////////////////////////////////////////////////////////////////////////////
// ------------------------- Basic Blue Ship --------------------------------- //
////////////////////////////////////////////////////////////////////////////////

// Makes a new, boring ship that just sits there.
BasicBlueShip::BasicBlueShip(Weapon *weapon, Hull *hull, Shield *shield) :
	Ship("./art/basicblueship.3DS", weapon, hull, new CargoBay(), shield, maxFuel(), 0.0, 0.0, 1.0)
{
	ai = new ShipAI(this, blueFactionInfo);
	faction = blueFactionInfo;
}

BasicBlueShip::~BasicBlueShip()
{
	vector<Ship*>::iterator startIterator;
	vector<Ship*>::iterator tempIterator;
	for( tempIterator = enemyShips.begin(); tempIterator != enemyShips.end(); tempIterator++ ) {
		if(*tempIterator == this)
		{
			enemyShips.erase(tempIterator);
			//cout << "erased" << endl;
			break;
		}
    }
}

// Draws the ship.
void BasicBlueShip::draw(int pass)
{
	glPushMatrix();

	// Draw the current path
/*	glPushMatrix();
	glColor3d(1.0, 0.0, 0.0);
	vector<Vec3> currentPath = ai->getPath();
	glBegin(GL_LINES);
	for(int i = 0; i < static_cast<int>(currentPath.size()) - 1; i++)
	{
		Vec3 start = currentPath.at(i);
		Vec3 end = currentPath.at(i + 1);
		glVertex3d(start.x(), start.y(), start.z());
		glVertex3d(end.x(), end.y(), end.z());
	}
	glEnd();
	glPopMatrix();
*/
	glTranslated(position.x(), position.y(), position.z());

	// direction rotation
	glMultMatrixd(lcs.array());

	if (pass == 1)
	{
		if (modelLoaded)
		{
			glColor3d(1,1,1);
			model.Draw();  // Renders the model to the screen
		}
		else
		{
			// Set color to blue.
			glColor3f(0.0, 0.0, 1.0);
			glutWireCone(2,4,5,1);
		}
	}
	else
	{
		// Draw that neat shield-bubble around the ship.
		shield->draw();
		tractorBeam->draw();
	}

	glPopMatrix();
}

void BasicBlueShip::hits(Object *o)
{
	Object::hits(o);

	// Jam:
	// This check allows bullets NOT to hurt the ship they came from.
	if (o->shouldHurt(this))
	{
		// Jam:
		// This is a constant right now.  Later it will be a function
		// of different things like hull strength and shields.
		hurt(200);
	}
	//cout << "Enemy ship hp: " << getHlth() << ", Shield: " << shield->getHlth() << endl;
	// Make it turn on player
	if(faction->getAttitude(PLAYER) == NEUTRAL && o->aiShouldConsider())
	{
		faction->setAttitude(ENEMY, PLAYER);
	}
}

void BasicBlueShip::destroy()
{
	soundFactory->play("explosion-ship");
	//cout << "Destroyed" << endl;
	Mission *m = NULL;
	for(unsigned int i = 0; i < missionsOn.size(); i++)
	{
		m = missionsOn.at(i);
		if(m->getID() == 1)
		{
			m->getObjective(0)->incrementKills();
		}
	}
	delete this;
}

