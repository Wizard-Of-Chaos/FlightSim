#pragma once

#ifndef FACTIONCOMPONENT_H
#define FACTIONCOMPONENT_H
#include "BaseHeader.h"

//The different types of factions in the game. Right now, it's just
//"neutral", "hostile", and "player" (friendly).
enum FACTION_TYPE
{
	FACTION_NEUTRAL = 1,
	FACTION_PLAYER = 2,
	FACTION_HOSTILE = 4
};

/*
* The faction component holds the type of faction (neutral, friendly, etc)
* as well as the factions that it's hostile to as an unsigned int (checked with bitmasking).
* Frankly, there's no need for this with the current three factions, but it's designed to be easy
* to expand later.
* IsHostile and IsFriendly will return whether or not the thing is hostile to a given other faction component.
*/
struct FactionComponent
{
	FACTION_TYPE type;
	unsigned int hostileTo;
	unsigned int friendlyTo;
	//Checks whether or not the other faction component would be hostile to this one.
	bool isHostile(const FactionComponent* other) const {
		return !!(hostileTo & other->type);
	}
	//Checks whether or not the other faction component would be friendly to this one.
	bool isFriendly(const FactionComponent* other) const {
		return !!(friendlyTo & other->type);
	}
};
#endif 