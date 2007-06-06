#ifndef OBJECT_H
#define OBJECT_H

#include "Model_3DS.h"
#include "vec3.h"

struct Leaf;

class Object
{
private:
	Leaf *leaf;  // Leaf where this object resides.

	double red;
	double green;
	double blue;
	
	//double radius2;  // Radius squared.

protected:
	double radius;   // Radius of the bounding sphere.
	Vec3 position;
	Vec3 velocity;
	double damage;
	Model_3DS model;
	bool modelLoaded;

public:
	Object(char *modelName, double red, double green, double blue);
	Object(char *modelName,const Vec3& pos, const Vec3& v, double red, double green, double blue);
	Object(char *modelName,double red, double green, double blue, double px, double py, double pz, double vx, double vy, double vz);
	//Object(double px, double py, double pz, double vx, double vy, double vz);

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

	virtual void hurt(double d) { damage += d; };
	void push(Vec3 direction) { velocity += direction; };

	virtual bool collidesWith(Object *o);
	void drawOnMiniMap(double r);
	virtual void hits(Object *o);
	virtual bool shouldHurt(Object *o) { return false; };
	virtual bool canLandOn() { return false; };

	virtual double maxHlth() const { return 100; };
};

#endif
