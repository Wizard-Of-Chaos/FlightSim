#pragma once
#ifndef CAMPAIGN_H
#define CAMPAIGN_H
#include "BaseHeader.h"
#include "Scenario.h"
#include "AIComponent.h"
#include "WeaponInfoComponent.h"
#include "LoadoutData.h"
#include <unordered_map>
#include <list>

const u32 NUM_SCENARIO_OPTIONS = 4;

/*
* The campaign holds all the data necessary to run the current campaign. The current difficulty acts as a multiplier on how many hostiles there are,
* and the current encounter holds which encounter is happening in the sector (max of four before a large scale battle). It also includes the data for
* how much damage can be repaired to ships and how much ammunition is available.
* 
* Presently, only about half of the data is used, but we're working on it, dammit.
*/
struct Campaign
{
	Campaign() : currentDifficulty(1), currentEncounter(0), totalAmmunition(100), totalRepairCapacity(1000) {
	}
	u32 currentDifficulty;
	u32 currentEncounter;
	Scenario possibleScenarios[NUM_SCENARIO_OPTIONS];
	Scenario currentScenario;
	u32 totalAmmunition;
	f32 totalRepairCapacity;

	bool moved = false;

	ShipInstance playerShip;
	std::vector<ShipInstance> availableShips;
	std::vector<WeaponInfoComponent> availableWeapons;
	std::vector<WeaponInfoComponent> availablePhysWeapons;
};

#endif