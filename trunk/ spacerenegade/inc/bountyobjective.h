#ifndef BOUNTYOBJECTIVE_H
#define BOUNTYOBJECTIVE_H
#include <string>
using std::string;

//#include EnemyShip

class BountyObjective: public Objective
{
private:
	int bountyId;
	int num;

protected:
	bool isComplete();
	string getDescription();

public:
	BountyObjective(int bountyId, int num);
	virtual ~BountyObjective();
};

#endif
