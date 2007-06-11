#ifndef WEAPON_H
#define WEAPON_H

#include "object.h"


class Ship;


////////////////////////////////////////////////////////////////////////////////
// ----------------------------- Things to Shoot ---------------------------- //
////////////////////////////////////////////////////////////////////////////////

/*******************************
 * Super class of all things that can be shot (like bombs, missiles, lasers,
 * etc.).  This is draw and has collision detection performed upon it (hence
 * it extends Object).
 ******************************/
class Ammo : public Object
{
protected:
	Ship *shooter;     // The person that shot me.
	int ticks;         // How many ticks this has been alive (compared
	                   // against ttl()).
	bool killNextTick; // To avoid having hits() kill itself,
	                   // potentially causing a segfault.
public:
	Ammo(Ship *shooter);
	virtual ~Ammo() {};
	virtual std::string getType() const { return "Ammo"; };

	virtual void update();
	virtual void draw(int pass) { Object::draw(pass); };
	virtual void hits(Object *o);

	virtual double ttl() const { return 60; };   // Time to live in frames.
	virtual double speed() const { return 1; };  // How fast this travels.
	virtual int damage() const { return 5; }; // Amount of damage this does.
};

/**********************************
 * A basic bullet.  It shoots.  Yay.
 *************************/
class Bullet : public Ammo
{
public:
	Bullet(Ship *shooter);
	virtual ~Bullet() {};
	virtual std::string getType() const { return "Bullet"; };

	virtual void draw(int pass);
	virtual bool shouldHurt(Object *o) { return o != (Object*)shooter; };

	virtual double ttl() const { return 120; };   // Time to live in frames.
	virtual double speed() const { return 2; };   // How fast this travels.
	virtual int damage() const { return 10; }; // Amount of damage this does.
};

/**********************************
 * A basic Laser Beam.  It also shoots.  Yay.
 *************************/
class LaserBeam : public Ammo
{
	// Coordinates of the points of this object.
	Vec3 A,B,C,D,E;

public:
	LaserBeam(Ship *shooter);
	virtual ~LaserBeam() {};
	virtual std::string getType() const { return "LaserBeam"; };

	virtual void draw(int pass);

	virtual double ttl() const { return 20; };   // Time to live in frames.
	virtual double speed() const { return 10; }; // How fast this travels.
	virtual int damage() const { return 5; };   // Amount of damage this does.
};



////////////////////////////////////////////////////////////////////////////////
// --------------------------- Things to Shoot With ------------------------- //
////////////////////////////////////////////////////////////////////////////////

/******************************
 * This class represents Weapons.  Weapons are never visible and only add
 * (or subtract in the case of bad weapons) from the functionality and
 * abilities of the ship they are attached to.
 *****************************/
class Weapon
{
protected:
//	Ammo type;   // The type of ammo this guy shoots.
	double ammo; // Keeps track of how much ammo remains.

	Weapon(double ammo) : ammo(ammo) {};

public:
	virtual ~Weapon() {};
	virtual std::string getType() const { return "Weapon"; };

	virtual void fire(Ship *shooter) {};

	// Constants that are functions to get the nice inheritence functionality.
	virtual double rof() const { return 10; };   // Rate Of Fire in frames.
	virtual double roh() const { return 10; };   // Rate Of Heating.
	virtual double roc() const { return 1; };    // Rate Of Cooling.
	virtual double ttr() const { return 1000; }; // Time To Recover from overheating.

	virtual double maxHeat() const { return 100; };  // When it overheats and needs a rest.
	virtual double wrnHeat() const { return 80; };   // When it looses efficiency?
	virtual double maxAmmo() const { return 100; };  // Most ammo this can hold.
	virtual double getAmmo() const { return ammo; }; // Amount of ammo this now has.
};

/********************************
 * This class is a laser-shooter.
 *****************/
class Blaster : public Weapon
{
public:
	Blaster() : Weapon(maxAmmo()) {};
	virtual ~Blaster() {};
	virtual std::string getType() const { return "Blaster"; };

	virtual void fire(Ship *shooter);

	// Constants that are functions to get the nice inheritence functionality.
	virtual double rof() const { return 10; };   // Rate Of Fire in frames.
	virtual double roh() const { return 2; };    // Rate Of Heating.
	virtual double roc() const { return 1; };    // Rate Of Cooling.
	virtual double ttr() const { return 1000; }; // Time To Recover from overheating.

	virtual double maxHeat() const { return 100; }; // When it overheats and needs a rest.
	virtual double wrnHeat() const { return 80; };  // When it looses efficiency?
	virtual double maxAmmo() const { return 100; }; // Most ammo this can hold.
};

class Laser : public Weapon
{
public:
	Laser() : Weapon(maxAmmo()) {};
	virtual ~Laser() {};
	virtual std::string getType() const { return "Laser"; };

	virtual void fire(Ship *shooter);

	// Constants that are functions to get the nice inheritence functionality.
	virtual double rof() const { return 30; };    // Rate Of Fire in frames.
	virtual double roh() const { return 10; };    // Rate Of Heating.
	virtual double roc() const { return 2; };     // Rate Of Cooling.
	virtual double ttr() const { return 10000; }; // Time To Recover from overheating.

	virtual double maxHeat() const { return 1000; }; // When it overheats and needs a rest.
	virtual double wrnHeat() const { return 800; };  // When it looses efficiency?
	virtual double maxAmmo() const { return 100; };  // Most ammo this can hold.
};



////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Tractor Beams ---------------------------- //
////////////////////////////////////////////////////////////////////////////////

class TractorBeam
{
protected:
	bool on;
	TractorBeam() : on(false) {};

public:
	virtual ~TractorBeam() {};
	virtual void draw() = 0;

	virtual void activate() { on = true; };
	virtual void deactivate() { on = false; };
	virtual void toggle() { on = !on; };
	virtual bool activated() { return on; };

	virtual double range() { return 10; };
	virtual double strength() { return 10; };
};

class BasicTractorBeam : public TractorBeam
{
	// Points for the tractorbeam.
	Vec3 a,b,c,d,e,f,g,h;
	Vec3 A,B,C,D,E,F,G,H;

public:
	BasicTractorBeam();
	virtual ~BasicTractorBeam() {};

	virtual void draw();
};

#endif

