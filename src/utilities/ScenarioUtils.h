#pragma once
#ifndef SCENARIOUTILS_H
#define SCENARIOUTILS_H
#include "BaseHeader.h"
#include "Scenario.h"

Scenario randomScenario();

void buildScenario(Scenario& scenario);
void setScenarioType(Scenario& scenario);
void setEnvironment(Scenario& scenario);

void setObstaclePositions(Scenario& scenario);

void buildAsteroidField(Scenario& scenario);
void setKillHostilesScenario(Scenario& scenario);
#endif 