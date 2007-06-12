#ifndef OBJECTIVE_H
#define OBJECTIVE_H
#include <string>

using std::string;

class Objective
{
protected:
	Objective() {};

public:
	// Listener of some sort
	virtual bool isComplete() { return false; };
	virtual string getDescription()
	{ return "getDescription";}; // (Gum) It's 3:30 AM and final projects/exams are breathing down my neck and I don't need C++ eating my brain right now when I'm trying to do a simple task like inheritance and I've tried about a dozen combinations of public, virtual, type-cast, that little star * thing that has all these meanings and it's still not doing the BountyObjective getDescription because there's no good tutorials online that don't look like a 1993 geocities website and excuse me, I believe my heart has just exploded
							 // ex. "2 out of 3 pirates destroyed."
							 //     "Deliver package to Ganymede."
	virtual void incrementKills() { };
};

class BountyObjective : public Objective
{
private:
	int bountyId; // id of enemy ship
	int num; // number of enemies to kill
	int bountiesKilled;
	string shipName;

public:
	BountyObjective();
	BountyObjective(string targetShipNames, int bountyId, int num);
	virtual ~BountyObjective();
	virtual void incrementKills() { bountiesKilled++; };

	virtual bool isComplete();
	virtual string getDescription();
};

#endif

