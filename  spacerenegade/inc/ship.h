#ifndef SHIP_H
#define SHIP_H

#include <string>
#include <AL/alut.h>
#include "Model_3DS.h"
#include "object.h"
#include "vec3.h"
#include "weapon.h"
#include "hull.h"
#include "planet.h"
#include "ai.h"

using std::string;

// General form of ships.  Rather abstract.
class Ship : public Object
{
	friend class ShipAI;

protected:
	// This is an orthonormal rotation matrix that describes the
	// orientation of the ship.  With this matrix you can do
	// pitch, roll, and yaw all "correctly" (i.e. as you'd
	// expect them to look if you were really flying around in
	// a spaceship) and you can easily get the orientation of
	// the ship by selecting good values.
	Matrix lcs;  // Local Coordinate System.

	Weapon *weapon; // What this ship shoots with.
	Hull *hull;
	CargoBay *bay;
	Shield *shield;
	TractorBeam *tractorBeam;
	double fuel;   // Amount of fuel left.
	ShipAI* ai;
	bool exploding; // If true, the ship will explode

	Ship(char* modelName, Weapon *weapon, Hull *hull, CargoBay *bay, Shield *shield,
			double fuel, double red, double green, double blue);

	GLdouble pitchF[16];
	GLdouble pitchB[16];
	GLdouble yawL[16];
	GLdouble yawR[16];
	GLdouble rollL[16];
	GLdouble rollR[16];

public:
	virtual ~Ship() { delete weapon; delete hull; delete bay; delete shield; delete tractorBeam; if(ai) delete ai; };
	//virtual ~Ship();

	// Rate functions.
	virtual double roa() const { return 0.005; }; // Rate of Acceleration
	virtual double rod() const { return 0.001; }; // Rate of Deceleration
	virtual double ros() const { return 0.95; };  // Rate of Stabilization
	virtual double rot() const { return 0.02; };  // Rate of Turn

	virtual string getType() const { return "Ship"; };

	Vec3 getDir() const { return Vec3(lcs[8],lcs[9],lcs[10]); };
	Vec3 getUp() const { return Vec3(lcs[4],lcs[5],lcs[6]); };
	Vec3 getLeft() const { return Vec3(lcs[0],lcs[1],lcs[2]); };
	double getRadiansRotated() const; // Number of radians the ship has rolled, Counterclockwise
	ShipAI* getAI() const { return ai; };

	virtual void update();
	virtual void draw(int pass);
	virtual void hurt(double d);
	virtual void fire();
	virtual void destroy() {};

	// Special?
	void toggleTractorBeam() { tractorBeam->toggle(); };
	void eliminateOldInertia(); // Accelerates in a direction to eliminate inertia that is not in the current direction
	void setDir(Vec3& newDir);  // Sets the new direction for the ship

	// Movement.
	virtual void accelerate() { velocity += getDir() * roa(); };
	virtual void decelerate() { velocity -= getDir() * rod(); };
	virtual void stabilize();
	virtual void pitchBack() { lcs *= pitchB; };
	virtual void pitchForward() { lcs *= pitchF; };
	virtual void yawLeft()   { lcs *= yawL; };
	virtual void yawRight()  { lcs *= yawR; };
	virtual void rollLeft()  { lcs *= rollL; };
	virtual void rollRight() { lcs *= rollR; };

	// Status stuff.
	virtual double maxHlth() const { return hull->maxHlth(); };
	virtual double maxCBay() const { return bay->capacity(); };
	virtual double maxFuel() const { return 10000; };
	virtual double maxAmmo() const { return weapon->maxAmmo(); };
	virtual double getHlth() const { return hull->getHlth(); };
	virtual double getWght() const { return bay->getWeight(); };
	virtual double getFuel() const { return fuel; };
	virtual double getAmmo() const { return weapon->getAmmo(); };
	virtual double hlthPcnt() const { return hull->hlthPcnt(); };
	virtual double cbayPcnt() const { return bay->getWeight() / bay->capacity(); };
	virtual double fuelPcnt() const { return fuel / maxFuel(); };
	virtual double ammoPcnt() const { return weapon->getAmmo() / weapon->maxAmmo(); };
	virtual double shldPcnt() const { return shield->hlthPcnt(); };
	virtual void addToHlth (int num) { hull->addToHlth(num); };
	virtual void addToCBay (double w) { bay->add(w); };
	virtual void addToFuel (int num) { fuel += num; };
	virtual void addToAmmo (int num) { weapon->addToAmmo(num); };
	//virtual double explodeTime() const { return
};



////////////////////////////////////////////////////////////////////////////////
// ------------------------ Player's Ship ----------------------------------- //
////////////////////////////////////////////////////////////////////////////////

// Interface for the player's ship.  Very specific to the needs of the player.
class PShip : public Ship
{
private:
	//bool lightRnotL;
	//int lightTime;
	GLTexture skymap;
	bool skymapLoaded;

	int credits;

	int fuelCost;
	int hlthCost;
	int ammoCost;

public:
	PShip(Weapon *weapon, Hull *hull, Shield *shield);
	virtual ~PShip() {};
	virtual string getType() const { return "PShip"; };
	virtual void hits(Object *o);
	virtual void update();
	virtual void draw(int pass);
	void drawReticle();
	virtual void fire();
	virtual void destroy();

	// Ship methods that need some restrictions put on them.
	virtual void accelerate();
	virtual void decelerate();
	virtual void stabilize();
	virtual void yawLeft();
	virtual void yawRight();
	virtual void rollLeft();
	virtual void rollRight();
	virtual void pitchBack();
	virtual void pitchForward();

	void refuel();
	void reload();
	void heal();
	int getProfit();
	void sellMaterials();
	void addCredits(int amt);

	int costToRefuel() { return static_cast<int>(fuelCost * (maxFuel() - fuel)); };
	int costToReload() { return static_cast<int>(ammoCost * (maxAmmo() - getAmmo())); };
	int costToHeal() { return static_cast<int>(hlthCost * (maxHlth() - getHlth())); };

	int getCredits() { return credits; };
	void addToCredits(int c) { credits += c; };
	bool canAfford(int c) { return (credits >= c); };

	// Rate functions inherited from Ship. They act as inheritable constants
	// for now, later they'll be based on engines and such.
	virtual double roa() const { return 0.005; };  // Rate of Acceleration
	virtual double rod() const { return 0.001; };  // Rate of Deceleration
	virtual double ros() const { return 0.95; };  // Rate of Stabilization
	virtual double rot() const { return 0.02; };  // Rate of Turn

	// Status of the ship. Later these will be affected by the
	// capacity of the ship and such.
	virtual double maxFuel() const { return 10000; };
};



////////////////////////////////////////////////////////////////////////////////
// ------------------------ Basic Red Ship ---------------------------------- //
////////////////////////////////////////////////////////////////////////////////

// Interface for the the basic red ship.
class BasicRedShip : public Ship
{
public:
	BasicRedShip(Weapon *weapon, Hull *hull, Shield *shield);
	virtual ~BasicRedShip();
	virtual string getType() const { return "BasicRedShip"; };

	//virtual void update();
	virtual void draw(int pass);
	virtual void hits(Object* o);

	// Note: AI has infinite fuel
	virtual void rollLeft()  {};
	virtual void rollRight() {};

	virtual void destroy();

	// Rate functions inherited from Ship
	virtual double roa() const { return 0.005; };  // Rate of Acceleration
	virtual double rod() const { return 0.001; };  // Rate of Deceleration
	virtual double ros() const { return 0.95; };  // Rate of Stabilization
	virtual double rot() const { return 0.02; };  // Rate of Turn

	// Status of the ship.
	virtual double maxHlth() const { return 500; };
	virtual double maxFuel() const { return 10000; };
	virtual double maxAmmo() const { return 100; };
};

////////////////////////////////////////////////////////////////////////////////
// ------------------------ Basic Blue Ship ---------------------------------- //
////////////////////////////////////////////////////////////////////////////////

// Interface for the the basic blue ship.
class BasicBlueShip : public Ship
{
public:
	BasicBlueShip(Weapon *weapon, Hull *hull, Shield *shield);
	virtual ~BasicBlueShip();
	virtual string getType() const { return "BasicRedShip"; };

	//virtual void update();
	virtual void draw(int pass);
	virtual void hits(Object* o);

	virtual void rollLeft()  {};
	virtual void rollRight() {};

	virtual void destroy();

	// Rate functions inherited from Ship
	virtual double roa() const { return 0.005; };  // Rate of Acceleration
	virtual double rod() const { return 0.001; };  // Rate of Deceleration
	virtual double ros() const { return 0.95; };  // Rate of Stabilization
	virtual double rot() const { return 0.02; };  // Rate of Turn

	// Status of the ship.
	virtual double maxHlth() const { return 1000; };
	virtual double maxFuel() const { return 10000; };
	virtual double maxAmmo() const { return 100; };
};

#endif

