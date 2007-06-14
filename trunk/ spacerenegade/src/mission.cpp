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
			title = "Help Crooky get his revenge!";
			briefing = "My name is Crooky and I want you to kill reds. Reds killed my pa. You kill reds now plz kthxbai. My name is Crooky and I want you to kill reds. Reds killed my pa. You kill reds now plz kthxbai. My name is Crooky and I want you to kill reds. Reds killed my pa. You kill reds now plz kthxbai. My name is Crooky and I want you to kill reds. Reds killed my pa. You kill reds now plz kthxbai. My name is Crooky and I want you to kill reds. Reds killed my pa. You kill reds now plz kthxbai. ";
			objectives.push_back(new BountyObjective("Red Ships", 0, 2));

			reward = 5000;
			completed = false;
			break;

		case 1:
			title = "Help Bumbum get his revenge!";
			briefing = "My name is Bumbum and I want you to kill blues. Blues killed my ma. You kill blues now plz kthxbai. ";
			objectives.push_back(new BountyObjective("Blue Ships", 1, 2));

			reward = 5000;
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
	drawText(515,725, insertNewLines(briefing, 41), Color(1,1,1), false);
}

void Mission::acceptMission()
{

}

bool Mission::isComplete() const
{
	bool complete = true;
	for(unsigned int i = 0; i < objectives.size(); i++)
	{
		complete = complete && objectives.at(i)->isComplete();
	}
	return complete;
}

// Automatically inserts "\" at every width'th character,
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
			spaceIndex = chunk.find_last_of(" ");
			chunk = chunk.substr(0, spaceIndex);
			newB.append(chunk + "\n");
			k += spaceIndex + 1;
		}
	}
	return newB;
}
