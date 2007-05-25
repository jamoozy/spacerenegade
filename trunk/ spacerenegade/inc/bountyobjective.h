#ifndef BOUNTYOBJECTIVE_H
#define BOUNTYOBJECTIVE_H
#include <string>
using std::string;

//#include EnemyShip

class BountyObjective: public Objective
{
private:
	int bountyId; // id of enemy ship
	int num; // number of enemies to kill

public:
	BountyObjective();
	BountyObjective(int bountyId, int num);
	virtual ~BountyObjective();

	bool isComplete();
	string getDescription();
};

#endif
