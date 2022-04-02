#pragma once
#ifndef SCENARIO_H
#define SCENARIO_H
#include "BaseHeader.h"
#include "GameFunctions.h"

class GameController;
class SceneManager;

enum SCENARIO_TYPE
{
	SCENARIO_KILL_HOSTILES,
	SCENARIO_DESTROY_OBJECT,
	SCENARIO_SALVAGE
};

#define SCENARIO_MAX_OBJECTIVES 10

struct Scenario
{
	Scenario(SCENARIO_TYPE type, u32 objCount, vector3df playerStart, vector3df enemyStart) :
		type(type), objectiveCount(objCount), playerStartPos(playerStart), enemyStartPos(enemyStart), complete(false), activeObjectives(objCount)
	{
		for (u32 i = 0; i < SCENARIO_MAX_OBJECTIVES; ++i) {
			objectives[i] = INVALID_ENTITY;
		}
	}
	SCENARIO_TYPE type;
	u32 objectiveCount;
	u32 activeObjectives;
	EntityId objectives[SCENARIO_MAX_OBJECTIVES];
	vector3df playerStartPos;
	vector3df enemyStartPos;
	bool complete;
};
#endif 