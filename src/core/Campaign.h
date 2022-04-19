#pragma once
#ifndef CAMPAIGN_H
#define CAMPAIGN_H
#include "BaseHeader.h"
#include "ECS.h"
#include "Scenario.h"
#include "AIComponent.h"
#include "WeaponInfoComponent.h"
#include <unordered_map>

struct Wingman
{
	u32 id;
	std::string name;
	std::string desc;
	AI_TYPE aiType;
	LoadoutData loadout;
};

struct Campaign
{
	u32 currentDifficulty;
	u32 currentEncounter;
	Scenario possibleScenarios[4];
	Scenario currentScenario;
	u32 totalAmmunition;
	f32 totalRepairCapacity;

	std::unordered_map<u32, ShipData*> unlockedShips;
	std::unordered_map<u32, WeaponData*> unlockedWeps;
	std::unordered_map<u32, WeaponData*> unlockedPhysWeps;
};

#endif