#ifndef FACTIONINFO_H
#define FACTIONINFO_H

#include <vector>
using std::vector;

// Possible factions that an object can belong to
enum Faction { PLAYER, RED, BLUE, WHITE, OTHER };
// Possible attitudes for a faction
enum Attitude { FRIEND, NEUTRAL, ENEMY };

/* Information for each faction.  There should be one set of FactionInfo for
   each faction on each mission. */
class FactionInfo
{
private:
	Faction myFaction;
	// A vector of attitudes for each faction
	vector<Attitude> attitudes;
	static const unsigned int NUM_FACTIONS;

public:
	// Constructors/Deconstructors
	FactionInfo(Faction f, const vector<Faction>& friends,
		const vector<Faction>& neutrals, const vector<Faction>& enemies);

	// Getters
	Faction getFaction() const { return myFaction; };
	vector<Faction> getFactionsOfAttitude(Attitude a) const;
	Attitude getAttitude(Faction f) const { return attitudes.at(f); };

	// Setters
	void setAttitude(Attitude newAttitude, Faction faction) { attitudes[faction] = newAttitude; };
	void setAttitude(Attitude newAttidude, const vector<Faction>& factions);
};

#endif // FACTIONINFO_H
