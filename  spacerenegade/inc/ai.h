#ifndef AI_H
#define AI_H

#include "object.h"
#include "ship.h"
#include "factionInfo.h"
#include "vec3.h"
#include "ou_thread.h"

using namespace openutils;

// Base Artificial Intelligence for ships
class ShipAI
{
public:
	// The state that an AI can be in.  The current state will determine the current behavior
	enum Mode { WAITING, PATROL, FOLLOW, ATTACK, GUARD };

protected:
	Thread *updateThread; // Thread that handles processing of AI
	Ship* pilotedShip; // Ship that this AI controls
	FactionInfo* myFactionInfo;
	double engagementRadius; // Maximum distance that the AI will pursue an enemy
	Mode currentMode;
	Object* target; // An object that the ship is paying attention to, such as to attack or follow
	Vec3 destination; // Current place where the ship is heading
	double proximityDistance;
	std::vector<Vec3> path; // current path that the ship is following
	int fireCoolDown; // Number of ticks before the ai can fire again.
	int updateCountDown; // Number of ticks before updating
	// Behavior attributes.  All doubles are on a scale from 0 to 1
	//double aggressiveness; // Aggressive enemies are more dedicated to attacking than evading

	bool goTo(const Vec3& pos); // Maneuvers ship to go towards position.  Return true if within proximityDistance
	/* These functions turn the ship in the necessary direction with the up 
		vector as (0, 1, 0).  Thus, if left() is called on the ship when it is 
		rotated 90 degrees, pitch back the ship. */
	void left();
	void right();
	void up();
	void down();

public:
	// Constructors/Deconstructors
	ShipAI(Ship* myShip, FactionInfo* myFaction);
	virtual ~ShipAI();

	// Getters
	virtual Ship* getShip() { return pilotedShip; };
	virtual Faction getFaction() { return myFactionInfo->getFaction(); };
	virtual std::vector<Vec3> getPath() { return path; };

	// Other Methods
	virtual void update(); // Process logic, such as moving the ship and firing


};

// Thread for handling the updating of the AI
class ShipAIThread : public Thread
{
	ShipAI* ai;
public:
	ShipAIThread(ShipAI *myAI) : ai(myAI) {}

	void run() { ai->update(); };
};

#endif // ai.h
