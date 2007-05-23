#ifndef WEAPON_H
#define WEAPON_H

#include "ship.h"
#include "object.h"


class Weapon : public Object
{
protected:
	Ship *shooter;    // The person that shot me.
	unsigned int ttl; // Time to live in frames
	static const double WEAPON_SPEED;

public:
	Weapon(Ship *shooter, unsigned int ttl);
	virtual ~Weapon();

	virtual void draw();

	virtual void hits(Object *o);
	virtual void selfDistruct();
};

#endif
