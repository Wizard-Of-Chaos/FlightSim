#pragma once
#ifndef SCENARIO_H
#define SCENARIO_H
#include "BaseHeader.h"
#include <unordered_map>

//Different types of scenario available.
enum SCENARIO_TYPE
{
	SCENARIO_KILL_HOSTILES,
	SCENARIO_DESTROY_OBJECT,
	SCENARIO_SALVAGE,
	SCENARIO_MAX_TYPES,
	SCENARIO_SCRAMBLE, // this is outside because we don't want this to generate when creating random scenarios
	SCENARIO_CARRIER_BATTLE,
	SCENARIO_NOT_LOADED
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

//Strings for loading a scenario from file.
const std::unordered_map<SCENARIO_TYPE, std::string> scenarioStrings = {
	{SCENARIO_KILL_HOSTILES, "killHostiles"},
	{SCENARIO_SALVAGE, "salvage"},
	{SCENARIO_DESTROY_OBJECT, "destroyObject"},
	{SCENARIO_SCRAMBLE, "scramble"},
	{SCENARIO_CARRIER_BATTLE, "carrierBattle"}
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
	{ }
	Scenario(SCENARIO_TYPE type, SCENARIO_ENVIRONMENT env, u32 objCount, vector3df playerStart, vector3df enemyStart) :
		type(type), objectiveCount(objCount), playerStartPos(playerStart), enemyStartPos(enemyStart), complete(false),
		environment(env) { }
	SCENARIO_TYPE type;
	SCENARIO_ENVIRONMENT environment;
	u32 detectionChance;

	std::string location;
	std::string description;

	std::vector<vector3df> obstaclePositions;

	u32 objectiveCount;

	vector3df playerStartPos;
	vector3df enemyStartPos;

	bool complete;

	u32 ammoRecovered;
	f32 resourcesRecovered;
	u32 maxWepsRecovered;
	u32 maxShipsRecovered;

	bool detected() {
		u32 roll = std::rand() % 100;
		if (roll < detectionChance) return true;
		return false;
	}
};
#endif 