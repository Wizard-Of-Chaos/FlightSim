#pragma once
#ifndef SCENARIOUTILS_H
#define SCENARIOUTILS_H
#include "BaseHeader.h"
#include "Scenario.h"
#include "CarrierUtils.h"
#include "ObjectiveComponent.h"

//Generates a random scenario.
Scenario randomScenario(bool scramble = false);
//Generates a scramble scenario for if the player has been detected.
Scenario scrambleScenario();

//Culls the scenario's obstacle list of obstacles from the given position in a sphere within the given radius.
void cullObstacleLocationsFromPosition(Scenario& scenario, vector3df pos, f32 rad);
//Culls the scenario's obstacle list of obstacles from around the player and enemy positions.
void cullStartPosObstacleLocations(Scenario& scenario);

//Builds the given scenario.
void buildScenario(Scenario& scenario);
//Sets up the type of scenario (the objectives) and places them appropriately.
void setScenarioType(Scenario& scenario);
//Sets up the scenario's environment and places obstacles. Call this AFTER the type setup - one of the things it does
//is cull the obstacle positions so there aren't any too close to the player / enemy spawns.
void setEnvironment(Scenario& scenario);
//Sets the obstacle positions in a sphere around the scenario. Called during scenario generation.
void setObstaclePositions(Scenario& scenario);

//Builds an asteroid field.
void buildAsteroidField(Scenario& scenario);
//Builds a gas field.
void buildGasField(Scenario& scenario);
//Builds a debris field.
void buildDebrisField(Scenario& scenario);

//Sets up a kill all hostiles scenario and places the enemies.
void setKillHostilesScenario(Scenario& scenario);
//Sets up a scramble scenario and places the carrier.
void setScrambleScenario(Scenario& scenario);

//A switch statement checking whether or not the objective has been completed.
bool isObjectiveCompleted(flecs::entity id);
#endif 