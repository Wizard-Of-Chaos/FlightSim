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

class SceneManager;
class GameController;
class GameStateController;

/*
* A big file of convenience functions that are used with the game.
*/

//Sets the default meshes for objects.
void setDefaults(SceneManager* manager);

//Translates a bullet vector to an irrlicht vector.
vector3df btVecToIrr(btVector3 vec);
//Translates an irrlicht vector to a bullet vector.
btVector3 irrVecToBt(vector3df vec);
vector3df randomVector();
vector3df randomRotationVector();
vector3df randomVectorSetDistance(vector3df& other, u32 dist);

//Creates a faction component on the given entity set to neutral.
void initializeNeutralFaction(SceneManager* manager, EntityId id);
//Creates a faction component on the given entity set to hostile.
void initializeHostileFaction(SceneManager* manager, EntityId id);
//Creates a faction component on the given entity set to the player faction.
void initializePlayerFaction(SceneManager* manager, EntityId id);
//Changes the faction type and hostilities/friendlies of the given faction. Allegiances should be passed in as a bitmask.
void setFaction(FactionComponent* fac, FACTION_TYPE type, unsigned int hostilities, unsigned int friendlies);

//Creates a ship using the data at the given id in the GameStateController.
//Example: The "Tuxedo" ship has the id 0 in the GameStateController. Throw that ID in here and it will load the ship.
//Adds: ShipComponent, IrrlichtComponent
EntityId createShipFromId(u32 id, SceneManager* manager, vector3df position);
//Creates a default ship entity at the given position. Does not include hitbox, health, or other components.
//Does include two default weapon entities and an irrlicht component / ship component. Returns the ID.
EntityId createDefaultShip(SceneManager* manager, vector3df position);
//Creates a default obstacle (asteroid) at the given position. Includes health, irrlicht, rigid body, and faction components. Returns the ID.
EntityId createDefaultObstacle(SceneManager* manager, vector3df position, vector3df scale);
//Creates a default AI ship. Includes AI, ship, bullet, irrlicht, weapon, sensors, and health components. Returns the ID.
EntityId createDefaultAIShip(SceneManager* manager, vector3df position);

//Removes the given object from the scene.
void destroyObject(SceneManager* manager, EntityId id);

//Initializes a weapon component on the given ship at the given hardpoint from the ID given.
//Example: The "Plasma Blaster" has the weapon id 1. Pass in 1 to this function to load a Plasma Blaster onto the hardpoint.
bool initializeWeaponFromId(u32 id, SceneManager* manager, EntityId shipId, int hardpoint);
//Adds a default weapon to the given ship at the given hardpoint.
//Requires: Irrlicht component, ship component. Returns false without that.
bool initializeDefaultWeapon(SceneManager* manager, EntityId shipId, int hardpoint);
//Adds on a player component and an input component to a given ship.
//Requires: Irrlicht component. Returns false without that.
bool initializeDefaultPlayer(SceneManager* manager, EntityId shipId);
//Adds a health component to the given entity. 
//Doesn't require anything, technically, but you should really only use it on things what have rigid bodies and guns and stuff.
void initializeDefaultHealth(SceneManager* manager, EntityId objectId);
//Sets up the HUD for the given player entity.
//Requires a player component; returns false otherwise.
bool initializeDefaultHUD(SceneManager* manager, EntityId playerId);
//Adds sensors to the given entity with default values. Requires an irrlicht component and a faction component.
void initializeDefaultSensors(SceneManager* manager, EntityId id);

IParticleSystemSceneNode* createShipJet(SceneManager* manager, ISceneNode* node, vector3df pos, vector3df dir);
//Adds the particle system to a given ship.
void initializeShipParticles(SceneManager* manager, EntityId id);

//Creates an explosion at the point that lasts for the duration.
EntityId explode(SceneManager* manager, vector3df position, f32 duration);

//Creates all the necessary details and components to set up the player's current loadout at the given position. Returns the
//EntityId of the player.
EntityId createPlayerShipFromLoadout(SceneManager* manager, vector3df pos);

bool initializeBtRigidBody(SceneManager* manager, EntityId entityId, btConvexHullShape shape, btVector3& scale);
bool initializeShipCollisionBody(SceneManager* manager, EntityId entityId, u32 shipId);
#endif