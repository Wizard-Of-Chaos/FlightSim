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
#include "HUDHeader.h"

/*
* A big file of convenience functions that are used with the game.
*/

//Sets the default meshes for objects.
void setDefaults();

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
void destroyObject(EntityId id);

//Adds on a player component and an input component to a given ship.
//Requires: Irrlicht component. Returns false without that.
bool initializeDefaultPlayer(EntityId shipId);
//Adds a health component to the given entity. 
void initializeHealth(EntityId id, f32 healthpool);
//Adds default health component.
void initializeDefaultHealth(EntityId objectId);
//Sets up the HUD for the given player entity.
//Requires a player component; returns false otherwise.
bool initializeDefaultHUD(EntityId playerId);

//Creates an explosion at the point that lasts for the duration.
EntityId explode(vector3df position, f32 duration, f32 scale, f32 radius, f32 damage, f32 force);
#endif