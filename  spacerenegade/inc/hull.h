
#ifndef HULL_H
#define HULL_H



////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Hulls ------------------------------------ //
////////////////////////////////////////////////////////////////////////////////

class Hull
{
protected:
	double damage;
	
	Hull() : damage(0) {};

public:
	virtual ~Hull() {};

	virtual void hurt(double amt) { damage += amt * (1-hap()); };
	virtual void heal() { damage = 0; };

	virtual double hap() const { return 0; };    // Hit Absorption Percentage.  How much of the damage this Hull absorbs.
	virtual double maxHlth() const { return 100; };
	virtual double getHlth() const { return maxHlth() - damage; };
	virtual double hlthPcnt() const  { return getHlth() / maxHlth(); };
	virtual void addToHlth(int num) { damage -= num; };
};

class BasicHull : public Hull
{
public:
	BasicHull() : Hull() {};
	virtual ~BasicHull() {};

	virtual double hap() const { return 0.02; };
	virtual double maxHlth() const { return 1000; };
};



////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Shields ---------------------------------- //
////////////////////////////////////////////////////////////////////////////////

class Shield
{
protected:
	double health;
	Shield(double health) : health(health) {};

public:
	virtual ~Shield() {};

	virtual void update();
	virtual void draw();
	virtual double hurt(double amt); // Hurts the shield, returns what it could not absorb.

	virtual double ror() const { return 0.2; };  // Rate of Regeneration;
	virtual double hap() const { return 0.00; };
	virtual double maxHlth() const { return 100; };
	virtual double getHlth() const { return health; };
	virtual double hlthPcnt() const { return getHlth() / maxHlth(); };
};

class BasicShield : public Shield
{
public:
	BasicShield() : Shield(maxHlth()) {};
	virtual ~BasicShield() {};

	virtual double hap() const { return 0.02; };
	virtual double maxHlth() const { return 100; };
};



////////////////////////////////////////////////////////////////////////////////
// -------------------------------- Cargo Bays ------------------------------ //
////////////////////////////////////////////////////////////////////////////////

// Cargo Bay with 0 capacity.  Only used for ships that don't actually have
// cargo bays.  In that sense, this is sort of a dummy object.  I could have
// just as well used NULL, but meh.
class CargoBay
{
protected:
	double weight;   // of contents.  Later this will be changed with something
	                 // more descriptive, like an array of objects.

public:
	CargoBay() : weight(0) {};  // Creates new, empty CargoBay.
	virtual ~CargoBay() {};

	// When weight gets changed later on, this should also change.
	virtual double getWeight() const { return weight; };

	// Returns true if the amount could be added or not.
	virtual bool add(double w);

	// Make this empty.
	virtual void empty() { weight = 0; };

	// Determind maximum you can fit in here.
	virtual double capacity() const { return 0; };
};

// Small Cargo Bay.
class SCargoBay : public CargoBay
{
public:
	SCargoBay() : CargoBay() {};  // Create new, empty SCargoBay.
	virtual ~SCargoBay() {};

	// Overwrite this, because this object can actually hold some stuff.
	virtual double capacity() const { return 1; }; // Metric tons?
};

#endif

