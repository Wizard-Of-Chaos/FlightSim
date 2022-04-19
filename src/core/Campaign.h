#pragma once
#ifndef CAMPAIGN_H
#define CAMPAIGN_H
#include "BaseHeader.h"
#include "ECS.h"
#include "Scenario.h"
#include "AIComponent.h"

struct Campaign
{
	u32 currentDifficulty;
	u32 currentEncounter;
	Scenario possibleScenarios[4];
	Scenario currentScenario;
	u32 totalAmmunition;
};

#endif