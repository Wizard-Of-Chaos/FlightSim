#pragma once
#ifndef SCENARIO_H
#define SCENARIO_H
#include "BaseHeader.h"
#include "GameFunctions.h"

//Different types of scenario available.
enum SCENARIO_TYPE
{
	SCENARIO_KILL_HOSTILES,
	SCENARIO_DESTROY_OBJECT,
	SCENARIO_SALVAGE,
	SCENARIO_MAX_TYPES,
	SCENARIO_SCRAMBLE, // this is outside because we don't want this to generate when creating random scenarios
	SCENARIO_NOT_LOADED
};

//Strings for loading a scenario from file.
const std::unordered_map<SCENARIO_TYPE, std::string> scenarioStrings = {
	{SCENARIO_KILL_HOSTILES, "killHostiles"},
	{SCENARIO_SALVAGE, "salvage"},
	{SCENARIO_DESTROY_OBJECT, "destroyObject"},
	{SCENARIO_SCRAMBLE, "scramble"}
};

//Different types of scenario environment available.
enum SCENARIO_ENVIRONMENT
{
	SCENENV_ASTEROID_FIELD,
	SCENENV_GAS_FIELD,
	SCENENV_EMPTY,
	SCENENV_DEBRIS_FIELD,
	SCENENV_SPACE_STATION,
	SCENENV_MAX_ENVIRONMENTS
};

//Detection chance associated with each environment.
const std::unordered_map<SCENARIO_ENVIRONMENT, u32> detectionChances = {
	{SCENENV_ASTEROID_FIELD, 15},
	{SCENENV_GAS_FIELD, 10},
	{SCENENV_EMPTY, 30},
	{SCENENV_DEBRIS_FIELD, 5},
	{SCENENV_SPACE_STATION, 35}
};

//Strings for loading and displaying a scenario environment.
const std::unordered_map<SCENARIO_ENVIRONMENT, std::string> scenarioEnvStrings = {
	{SCENENV_ASTEROID_FIELD, "Asteroid Field"},
	{SCENENV_GAS_FIELD, "Gas Field"},
	{SCENENV_EMPTY, "Empty Space"},
	{SCENENV_DEBRIS_FIELD, "Debris Field"},
	{SCENENV_SPACE_STATION, "Space Station"}
};

//Max amount of objectives possible (enemies to kill, things to blow up, etc).
const u32 SCENARIO_MAX_OBJECTIVES = 10;

/*
* Scenarios hold the information for... well, a scenario. They get randomly generated and include the objectives, player start,
* enemy start, environment, type, objective count, and others. When initialized (i.e. run by the actual game when the player hits start)
* the entities for the game get created at the positions listed here. The scenario will get constantly checked to see whether the objectives
* have been completed, and if so, the bool "complete" will be set to true and the scenario will end (either that or the player dies like a dog).
*/
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
	u32 detectionChance;

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