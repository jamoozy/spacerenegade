#include "GL/glut.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <cmath>
#include "display.h"
#include "menu.h"
#include "mission.h"
#include "objective.h"

using std::string;
using std::cout;
using std::endl;

extern Menu *menu;

void cleanup();

////////////////////////////////////////////////////////////////////////////////
// -------------------------- Mission Definition ---------------------------- //
////////////////////////////////////////////////////////////////////////////////

Mission::Mission(int id) : id(id), title("Title goes here"), briefing("Briefing goes here"),
	numObjs(0), reward(0), completed(false)
{
	switch (id)
	{
		case 0:
			title = "A Test of Honor";
			briefing = "Greetings, traveler. I understand you are looking for work.\n\nMy name is Lieutenant Crooky, and we are looking for more Blue soldiers. If you want to join our alliance, prove your devotion by destroying two Red ships. We'll reward not only with 10,000 credits, but an opportunity to one day lead the entire Blue faction into glory. ";
			completeBriefing = "Well done. You are clearly devoted to our cause. Expect to see more offers from us in the future, with higher stakes and higher rewards.  \n \n Here is 10,000 credits, as promised.";
			objectives.push_back(new BountyObjective("Red Ships", 0, 2));

			reward = 10000;
			completed = false;
			break;

		case 1:
			title = "Blood Red";
			briefing = "Hey, kid! Your face tells me you're looking for money. Well, we've got the money if you've got the guts.\n\nThey call me Bumbum the Corpulent, for I am the leader of the mightiest faction in the galaxy: The Reds. If you wanna join the party, you gotta have guts. Shoot down two Blue ships and you'll get 10,000 credits... and we may just call you a fellow Red. ";
			completeBriefing = "Ha! You sure know how to party. If you're hungry for more, drop by the mission board some other time.  \n \n Here's 10,000 clams. Don't spend it all in one place! HA HA HA.";
			objectives.push_back(new BountyObjective("Blue Ships", 1, 2));

			reward = 10000;
			completed = false;
			break;
	}
}

Mission::~Mission()
{
	while(!objectives.empty())
	{
		delete objectives.back();
		objectives.pop_back();
	}
}

void Mission::displayMissionBriefing()
{
	drawText(515,725, insertNewLines(getBriefing(), 41), Color(1,1,1), false);
}

void Mission::acceptMission()
{

}

bool Mission::isCompleted() const
{
	bool complete = true;
	for(unsigned int i = 0; i < objectives.size(); i++)
	{
		complete = complete && objectives.at(i)->isComplete();
	}
	return complete;
}

// Automatically inserts "\n" at every width'th character,
// formatting to break at spaces.
string Mission::insertNewLines(string b, int width)
{
	string newB = "";
	string chunk;
	unsigned int k = 0;
	int spaceIndex;
	while (k < b.length())
	{
		if ((k + width) > b.length()) // end of briefing
		{
			newB.append(b.substr(k, b.length()));
			k += width;
		}
		else
		{
			chunk = b.substr(k,width+1);
			spaceIndex = chunk.find_last_of("\n");
			if (spaceIndex > -1) // if there is a "\n" in this chunk (possible bug: if chunk is "blahblah\" and rest of string is "nblahblah". Oh well, easier to avoid than fix at the moment.)
			{
				chunk = chunk.substr(0, spaceIndex + 2);
				newB.append(chunk);
				k += spaceIndex + 2;
			}
			else
			{
				spaceIndex = chunk.find_last_of(" ");
				chunk = chunk.substr(0, spaceIndex);
				newB.append(chunk + "\n");
				k += spaceIndex + 1;
			}
		}
	}
	return newB;
}
