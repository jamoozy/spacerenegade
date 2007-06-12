#include "factionInfo.h"
#include <vector>

using std::vector;

////////////////////////////////////////////////////////////////////////////////
// --------------------------- Faction-Info --------------------------------- //
////////////////////////////////////////////////////////////////////////////////

const unsigned int FactionInfo::NUM_FACTIONS = 5;

// Constructors/Deconstructors
FactionInfo::FactionInfo(Faction f, const vector<Faction>& friends,
		const vector<Faction>& neutrals, const vector<Faction>& enemies) : 
		myFaction(f)
{
	attitudes.assign(NUM_FACTIONS, NEUTRAL);
	setAttitude(FRIEND, friends);
	setAttitude(NEUTRAL, neutrals);
	setAttitude(ENEMY, enemies);
	// Insure that this faction is friendly with members of the same faction
	setAttitude(FRIEND, myFaction);
}

// Getters
vector<Faction> FactionInfo::getFactionsOfAttitude(Attitude a) const
{
	vector<Faction> friends;

	for(unsigned int i = 0; i < NUM_FACTIONS; i++)
	{
		if(attitudes.at(i) == a)
			friends.push_back(static_cast<Faction>(i));
	}

	return friends;
}

// Setters
void FactionInfo::setAttitude(Attitude newAttidude, const vector<Faction>& factions)
{
	for(unsigned int i = 0; i < factions.size(); i++)
	{
		attitudes[factions.at(i)] = newAttidude;
	}
}
