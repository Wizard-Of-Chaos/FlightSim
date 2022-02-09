#pragma once

#ifndef FACTIONCOMPONENT_H
#define FACTIONCOMPONENT_H
#include "BaseHeader.h"
#include "ECS.h"

enum FACTION_TYPE
{
	FACTION_NEUTRAL = 1,
	FACTION_PLAYER = 2,
	FACTION_HOSTILE = 4
};

struct FactionComponent
{
	FACTION_TYPE type;
	unsigned int hostileTo;
	unsigned int friendlyTo;
	bool isHostile(FactionComponent* other) {
		if ((hostileTo & other->type) == hostileTo) return true;
		return false;
	}
	bool isFriendly(FactionComponent* other) {
		if ((friendlyTo & other->type) == friendlyTo) return true;
		return false;
	}
};
#endif 