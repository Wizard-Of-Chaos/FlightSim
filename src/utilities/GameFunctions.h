#pragma once

#ifndef GAMEFUNCTIONS_H
#define GAMEFUNCTIONS_H
#include "BaseHeader.h"
#include "IrrlichtComponent.h"
#include "HealthComponent.h"
#include "InputComponent.h"
#include "PlayerComponent.h"
#include "ShipComponent.h"
#include "WeaponInfoComponent.h"
#include "FactionComponent.h"

class SceneManager;

//Sets the default meshes for objects.
void setDefaults(SceneManager* manager);

//Translates a bullet vector to an irrlicht vector.
vector3df bulletVectorToIrrlicht(btVector3 vec);
//Translates an irrlicht vector to a bullet vector.
btVector3 irrlichtVectorToBullet(vector3df vec);
vector3df randomVector();
vector3df randomRotationVector();

//Creates a faction component on the given entity set to neutral.
void initializeNeutralFaction(SceneManager* manager, EntityId id);
//Creates a faction component on the given entity set to hostile.
void initializeHostileFaction(SceneManager* manager, EntityId id);
//Creates a faction component on the given entity set to the player faction.
void initializePlayerFaction(SceneManager* manager, EntityId id);
//Changes the faction type and hostilities/friendlies of the given faction. Allegiances should be passed in as a bitmask.
void setFaction(FactionComponent* fac, FACTION_TYPE type, unsigned int hostilities, unsigned int friendlies);

//Creates a default ship entity at the given position. Does not include hitbox, health, or other components.
//Does include two default weapon entities and an irrlicht component / ship component. Returns the ID.
EntityId createDefaultShip(SceneManager* manager, vector3df position);
//Creates a default obstacle (asteroid) at the given position. Includes health, irrlicht, and faction components. Returns the ID.
EntityId createDefaultObstacle(SceneManager* manager, vector3df position);
//Creates a projectile entity and shoots it in the indicated direction. Assigns information to the projectile (range, speed, etc)
//based on the weapon that shot it. Returns the ID of the projectile.
EntityId createProjectileEntity(SceneManager* manager, vector3df spawnPos, vector3df direction, EntityId weaponId);
//Creates a default AI ship. Includes AI, ship, bullet, irrlicht, weapon and health components. Returns the ID.
EntityId createDefaultAIShip(SceneManager* manager, vector3df position);

//Removes the given projectile from the scene.
void destroyProjectile(SceneManager* manager, EntityId projectile);
//Removes the given object from the scene.
void destroyObject(SceneManager* manager, EntityId id);

//Adds a default weapon to the given ship at the given hardpoint.
//Requires: Irrlicht component, ship component. Returns false without that.
bool initializeDefaultWeapon(SceneManager* manager, EntityId shipId, int hardpoint);
//Sets up a default hitbox (an axis-aligned bounding box) on the given object.
//Requires: Irrlicht component. Returns false otherwise.
bool initializeDefaultRigidBody(SceneManager* manager, EntityId objectId);
//Adds on a player component and an input component to a given ship.
//Requires: Irrlicht component. Returns false without that.
bool initializeDefaultPlayer(SceneManager* manager, EntityId shipId);
//Adds a health component to the given entity. 
//Doesn't require anything, technically, but you should really only use it on things what have rigid bodies and guns and stuff.
void initializeDefaultHealth(SceneManager* manager, EntityId objectId);
//Sets up the HUD for the given player entity.
//Requires a player component; returns false otherwise.
bool initializeDefaultHUD(SceneManager* manager, EntityId playerId);

#endif