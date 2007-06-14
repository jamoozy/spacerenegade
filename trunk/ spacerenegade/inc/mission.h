#ifndef MISSION_H
#define MISSION_H

#include <string>
#include <vector>
#include "objective.h"
#include "object.h"


class Mission
{
private:
	int id;
	string title; // Headline of mission ("Help Crooky get his revenge!")
	string briefing; // Detailed explanation of missions as told by client
	int numObjs; // number of objectives
	std::vector<BountyObjective*> objectives;
	int reward; // ex. 1000 credits
	bool completed; // all objectives have been met, and player should go to finishPlanet
	//boolean hasFailed; // one or more conditions have been failed (such as allowing a ship to die)
	// EventListener listens for objective-specific events (such as # of bounties killed)

	// Desired, but not achievable:
	//   Location startPlanet, finishPlanet; // Start & Finish locations (for delivery, escort, etc)
	//   Time deadline;

	string insertNewLines(string b, int width);

public:
	Mission();
	virtual ~Mission();
	Mission(int id); // probably only one or two missions in demo
	bool isCompleted() const      { return isComplete; };
	//bool hasFailed(); // won't use conditions in the demo
	string getTitle() const       { return title; };
	string getBriefing() const    { return briefing; };
	int getReward() const         { return reward; };
	int getID() const             { return id; };
	//Objective getObjectives();
	int getNumObjectives() const  { return objectives.size(); };
	Objective* getObjective(int i) { return objectives.at(i);};
	bool isComplete() const;
	//BountyObjective getObjective(int i) {return objectives[i];};

	void acceptMission();
	void displayMissionBriefing();
	int getNumObjs() {return objectives.size();};
};

#endif

