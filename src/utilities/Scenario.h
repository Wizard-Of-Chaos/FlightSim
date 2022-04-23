#pragma once
#ifndef SCENARIO_H
#define SCENARIO_H
#include "BaseHeader.h"
#include "GameFunctions.h"

enum SCENARIO_TYPE
{
	SCENARIO_KILL_HOSTILES,
	SCENARIO_DESTROY_OBJECT,
	SCENARIO_SALVAGE,
	SCENARIO_MAX_TYPES,
	SCENARIO_NOT_LOADED
};

const std::unordered_map<SCENARIO_TYPE, std::string> scenarioStrings = {
	{SCENARIO_KILL_HOSTILES, "killHostiles"},
	{SCENARIO_SALVAGE, "salvage"},
	{SCENARIO_DESTROY_OBJECT, "destroyObject"}
};

enum SCENARIO_ENVIRONMENT
{
	SCENENV_ASTEROID_FIELD,
	SCENENV_GAS_FIELD,
	SCENENV_EMPTY,
	SCENENV_DEBRIS_FIELD,
	SCENENV_SPACE_STATION,
	SCENENV_MAX_ENVIRONMENTS
};

const std::unordered_map<SCENARIO_ENVIRONMENT, std::string> scenarioEnvStrings = {
	{SCENENV_ASTEROID_FIELD, "Asteroid Field"},
	{SCENENV_GAS_FIELD, "Gas Field"},
	{SCENENV_EMPTY, "Empty Space"},
	{SCENENV_DEBRIS_FIELD, "Debris Field"},
	{SCENENV_SPACE_STATION, "Space Station"}
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
	Scenario(SCENARIO_TYPE type, SCENARIO_ENVIRONMENT env, u32 objCount, vector3df playerStart, vector3df enemyStart) :
		type(type), objectiveCount(objCount), playerStartPos(playerStart), enemyStartPos(enemyStart), complete(false), activeObjectiveCount(objCount),
		environment(env)
	{
		for (u32 i = 0; i < SCENARIO_MAX_OBJECTIVES; ++i) {
			objectives[i] = INVALID_ENTITY;
			objectivePos[i] = vector3df(0, 0, 0);
		}
	}
	SCENARIO_TYPE type;
	SCENARIO_ENVIRONMENT environment;

	std::vector<vector3df> obstaclePositions;
	u32 objectiveCount;
	u32 activeObjectiveCount;
	EntityId objectives[SCENARIO_MAX_OBJECTIVES];
	vector3df objectivePos[SCENARIO_MAX_OBJECTIVES];
	vector3df playerStartPos;
	vector3df enemyStartPos;
	bool complete;

	std::string location;
	std::string description;
};
#endif 