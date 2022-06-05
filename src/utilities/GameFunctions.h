#pragma once

#ifndef GAMEFUNCTIONS_H
#define GAMEFUNCTIONS_H
#include "BaseHeader.h"
#include "AttributeReaders.h"
#include "IrrlichtComponent.h"
#include "HealthComponent.h"
#include "InputComponent.h"
#include "PlayerComponent.h"
#include "ShipComponent.h"
#include "WeaponInfoComponent.h"
#include "FactionComponent.h"
#include "ExplosionComponent.h"
#include "AIComponent.h"
#include "HUDHeader.h"

/*
* A file of convenience functions that are used with the game.
*/

//Translates a bullet vector to an irrlicht vector.
vector3df btVecToIrr(btVector3 vec);
//Translates an irrlicht vector to a bullet vector.
btVector3 irrVecToBt(vector3df vec);
vector3df randomVector();
vector3df randomRotationVector();

//Gets a point in the defined sphere.
vector3df getPointInSphere(vector3df center, f32 radius);
//Checks if a point is in the defined sphere.
bool isPointInSphere(vector3df& point, vector3df center, f32 radius);

//Removes the given object from the scene.
void destroyObject(flecs::entity id);

//Adds on a player component and an input component to a given ship.
//Requires: Irrlicht component. Returns false without that.
bool initializeDefaultPlayer(flecs::entity shipId);
//Adds a health component to the given entity. 
void initializeHealth(flecs::entity id, f32 healthpool);
//Adds default health component.
void initializeDefaultHealth(flecs::entity objectId);
//Sets up the HUD for the given player entity.
//Requires a player component; returns false otherwise.
bool initializeDefaultHUD(flecs::entity playerId);

//Creates AI component with the assigned values.
void initializeAI(flecs::entity id, AI_TYPE type, f32 reactSpeed, f32 damageTolerance);
//Creates an AI component with default values.
void initializeDefaultAI(flecs::entity id);

//Creates an explosion at the point that lasts for the duration.
flecs::entity explode(vector3df position, f32 duration, f32 scale, f32 radius, f32 damage, f32 force);

//Gets the data from the player ship at the end of a scenario.
ShipInstance getEndScenarioData();
//Constructs a new ship instance.
ShipInstance newShipInstance();
//Initializes a new campaign with default values.
void initNewCampaign();

ShipInstance randomShipInstance();
WeaponInfoComponent randomWeapon();

#endif