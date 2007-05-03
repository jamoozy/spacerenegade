#ifndef OBJECT_H
#define OBJECT_H

#include "vec3.h"


class Object
{
protected:
	Vec3 position;

public:
	Object();
	~Object();
	void draw();
};
		
#endif
