#pragma once
#ifndef SHIPUTILS_H
#define SHIPUTILS_H
#include "BaseHeader.h"
#include "FactionComponent.h"
#include "ECS.h"

class SceneManager;
class GameController;
class GameStateController;

//Creates a ship using the data at the given id in the GameStateController.
//Example: The "Tuxedo" ship has the id 0 in the GameStateController. Throw that ID in here and it will load the ship.
//Adds: ShipComponent, IrrlichtComponent
EntityId createShipFromId(u32 id, SceneManager* manager, vector3df position);
//Creates a default ship entity at the given position. Does not include hitbox, health, or other components.
//Does include two default weapon entities and an irrlicht component / ship component. Returns the ID.
EntityId createDefaultShip(SceneManager* manager, vector3df position);
//Creates a default AI ship. Includes AI, ship, bullet, irrlicht, weapon, sensors, and health components. Returns the ID.
EntityId createDefaultAIShip(SceneManager* manager, vector3df position);
//Creates all the necessary details and components to set up the player's current loadout at the given position. Returns the
//EntityId of the player.
EntityId createPlayerShipFromLoadout(SceneManager* manager, vector3df pos);
//Adds on the ship's collision body (i.e., the rigid body component) to the entity.
bool initializeShipCollisionBody(SceneManager* manager, EntityId entityId, u32 shipId);

//Initializes a weapon component on the given ship at the given hardpoint from the ID given.
//Example: The "Plasma Blaster" has the weapon id 1. Pass in 1 to this function to load a Plasma Blaster onto the hardpoint.
bool initializeWeaponFromId(u32 id, SceneManager* manager, EntityId shipId, int hardpoint);
//Adds a default weapon to the given ship at the given hardpoint.
//Requires: Irrlicht component, ship component. Returns false without that.
bool initializeDefaultWeapon(SceneManager* manager, EntityId shipId, int hardpoint);

//Creates a faction component on the entity with the given hostilities, friendlies, and type.
void initializeFaction(SceneManager* manager, EntityId id, FACTION_TYPE type, u32 hostiles, u32 friendlies);
//Creates a faction component on the given entity set to neutral.
void initializeNeutralFaction(SceneManager* manager, EntityId id);
//Creates a faction component on the given entity set to hostile.
void initializeHostileFaction(SceneManager* manager, EntityId id);
//Creates a faction component on the given entity set to the player faction.
void initializePlayerFaction(SceneManager* manager, EntityId id);
//Changes the faction type and hostilities/friendlies of the given faction. Allegiances should be passed in as a bitmask.
void setFaction(FactionComponent* fac, FACTION_TYPE type, unsigned int hostilities, unsigned int friendlies);

//Adds sensors to the given entity. Requires a bullet and a faction component.
bool initializeSensors(SceneManager* manager, EntityId id, f32 range, f32 updateInterval);
//Adds sensors to the given entity with default values. Requires a bullet component and a faction component.
bool initializeDefaultSensors(SceneManager* manager, EntityId id);

//Adds shield component to the given entity.
void initializeShields(SceneManager* manager, EntityId id, f32 amount, f32 delay, f32 recharge);
//Adds default shield values.
void initializeDefaultShields(SceneManager* manager, EntityId objectId);

//Creates a particle jet for the ship at the given position.
IParticleSystemSceneNode* createShipJet(SceneManager* manager, ISceneNode* node, vector3df pos, vector3df dir);
//Adds the particle system to a given ship.
void initializeShipParticles(SceneManager* manager, EntityId id);
#endif 
