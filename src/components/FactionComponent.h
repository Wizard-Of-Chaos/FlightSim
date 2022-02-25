#pragma once

#ifndef FACTIONCOMPONENT_H
#define FACTIONCOMPONENT_H
#include "BaseHeader.h"
#include "ECS.h"

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
	bool isHostile(FactionComponent* other) {
		return !!(hostileTo & other->type);
	}
	bool isFriendly(FactionComponent* other) {
		return !!(friendlyTo & other->type);
	}
};
#endif 