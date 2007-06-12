#ifndef OBJECT_H
#define OBJECT_H

#include <AL/alut.h>
#include "Model_3DS.h"
#include "vec3.h"
#include "factionInfo.h"
#include "display.h"

struct Leaf;
extern SoundFactory *soundFactory;

class Object
{
private:
	Leaf *leaf;  // Leaf where this object resides.

	double red;
	double green;
	double blue;

	ALuint hitBuffer, hitSource;
	
	//double radius2;  // Radius squared.

protected:
	double radius;   // Radius of the bounding sphere.
	Vec3 position;
	Vec3 velocity;
	double damage;
	Model_3DS model;
	bool modelLoaded;
	FactionInfo* faction;

public:
	// These constructors are used for adding temporary dummy objects
	Object(char *modelName);
	Object(char *modelName, double objRadius);
	// These Constructors are used for adding real objects, which need a RGB value for the minimap
	Object(char *modelName, double red, double green, double blue);
	Object(char *modelName,const Vec3& pos, const Vec3& v, double red, double green, double blue);
	Object(char *modelName,double red, double green, double blue, double px, double py, double pz, double vx, double vy, double vz);
	//Object(double px, double py, double pz, double vx, double vy, double vz);

	virtual ~Object();
	virtual std::string getType() const { return "Object"; };

	virtual void update() { position += velocity; };
	virtual void draw(int pass) { };
	virtual void setAt(const Vec3& pos) { position = pos; };
	virtual void setAt(double x, double y, double z) { position = Vec3(x,y,z); };
	virtual void setRadius(double r) { radius = r; };
	virtual Vec3 getVel() const { return velocity; };                // Get current velocity.
	virtual Vec3 getPos() const { return position; };                // Get current position.
	virtual Vec3 getNextPos() const { return position + velocity; }; // Get position at next frame (assumes constant velocity).
	virtual double getRadius() const { return radius; };
	virtual FactionInfo* getFaction() const { return faction; };
	virtual bool aiShouldConsider() const { return false; };	// If true, ai ignores this.  Only bullets should be ignored.

	bool checkResidence();
	Leaf *getResidence() { return leaf; };
	void setResidence(Leaf *l) { leaf = l; };

	virtual void hurt(double d) 
	{ 
		damage += d;
	
		soundFactory->play("hit");
		//if (hitBuffer != AL_NONE)
		//	alSourcePlay(hitSource);
	};
	void push(Vec3 direction) { velocity += direction; };
	void stop() { velocity = Vec3(0,0,0); };

	virtual bool collidesWith(Object *o);
	void drawOnMiniMap(double r);
	virtual void hits(Object *o);
	virtual bool shouldHurt(Object *o) { return false; };
	virtual bool canLandOn() { return false; };

	virtual double maxHlth() const { return 100; };
};

#endif
