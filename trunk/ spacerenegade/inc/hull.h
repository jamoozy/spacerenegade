
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

	virtual double hap() const { return 0; };    // Hit Absorption Percentage.  How much of the damage this Hull absorbs.
	virtual double maxHlth() const { return 100; };
	virtual double getHlth() const { return maxHlth() - damage; };
	virtual double hlthPcnt() const  { return getHlth() / maxHlth(); };
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

#endif

