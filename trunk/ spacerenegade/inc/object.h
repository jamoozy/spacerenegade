#ifndef OBJECT_H
#define OBJECT_H

#include "Model_3DS.h"
#include "vec3.h"

struct Leaf;

class Object
{
	Leaf *leaf;  // Leaf where this object resides.

protected:
	double radius;   // Radius of the bounding sphere.
	//double radius2;  // Radius squared.

	Vec3 position;
	Vec3 velocity;

	int damage;

	Model_3DS model;
	bool modelLoaded;

public:
	Object(char *modelName);
	Object(char *modelName, const Vec3& pos, const Vec3& v);
	Object(char *modelName, double px, double py, double pz, double vx, double vy, double vz);
	virtual ~Object();
	virtual std::string getType() const { return "Object"; };

	virtual void draw() { position += velocity; };
	virtual void setAt(const Vec3& pos) { position = pos; };
	virtual void setAt(double x, double y, double z) { position = Vec3(x,y,z); };
	virtual Vec3 getVel() const { return velocity; };                // Get current velocity.
	virtual Vec3 getPos() const { return position; };                // Get current position.
	virtual Vec3 getNextPos() const { return position + velocity; }; // Get position at next frame (assumes constant velocity).

	bool checkResidence();
	Leaf *getResidence() { return leaf; };
	void setResidence(Leaf *l) { leaf = l; };

	void hurt(int d) { damage += d; };
	void push(Vec3 direction) { velocity += direction; };

	bool collidesWith(Object *o);
	virtual void hits(Object *o);

	virtual int maxHealth() const { return 100; };
};

#endif
