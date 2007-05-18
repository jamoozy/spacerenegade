#ifndef WEAPON_H
#define WEAPON_H

#include "ship.h"
#include "object.h"


class Weapon : public Object
{
protected:
	Ship *shooter;    // The person that shot me.
	static const double WEAPON_SPEED;

public:
	Weapon(Ship *shooter);
	virtual ~Weapon();

	virtual void draw();

	virtual bool collidesWith(Object *o);
};

#endif
