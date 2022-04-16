#pragma once
#ifndef SCENARIO_H
#define SCENARIO_H
#include "BaseHeader.h"
#include "GameFunctions.h"

enum SCENARIO_TYPE
{
	SCENARIO_NOT_LOADED,
	SCENARIO_KILL_HOSTILES,
	SCENARIO_DESTROY_OBJECT,
	SCENARIO_SALVAGE
};

const u32 SCENARIO_MAX_OBJECTIVES = 10;

struct Scenario
{
	Scenario() : complete(false)
	{
		for (u32 i = 0; i < SCENARIO_MAX_OBJECTIVES; ++i) {
			objectives[i] = INVALID_ENTITY;
			objectivePos[i] = vector3df(0, 0, 0);
		}
	}
	Scenario(SCENARIO_TYPE type, u32 objCount, vector3df playerStart, vector3df enemyStart) :
		type(type), objectiveCount(objCount), playerStartPos(playerStart), enemyStartPos(enemyStart), complete(false), activeObjectiveCount(objCount)
	{
		for (u32 i = 0; i < SCENARIO_MAX_OBJECTIVES; ++i) {
			objectives[i] = INVALID_ENTITY;
			objectivePos[i] = vector3df(0, 0, 0);
		}
	}
	SCENARIO_TYPE type;
	u32 objectiveCount;
	u32 activeObjectiveCount;
	EntityId objectives[SCENARIO_MAX_OBJECTIVES];
	vector3df objectivePos[SCENARIO_MAX_OBJECTIVES];
	vector3df playerStartPos;
	vector3df enemyStartPos;
	bool complete;
};
#endif 