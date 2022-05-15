#pragma once
#ifndef SHIPUTILS_H
#define SHIPUTILS_H
#include "BaseHeader.h"
#include "FactionComponent.h"
#include "LoadoutData.h"
#include "AIComponent.h"
#include "ShipParticleComponent.h"
#include "ECS.h"

//Creates a ship using the data at the given id in the GameStateController.
//Example: The "Tuxedo" ship has the id 0 in the GameStateController. Throw that ID in here and it will load the ship.
//Adds: ShipComponent, IrrlichtComponent
EntityId createShipFromId(u32 id, vector3df position, vector3df rotation);
//Creates a default ship entity at the given position. Does not include hitbox, health, or other components.
//Does include two default weapon entities and an irrlicht component / ship component. Returns the ID.
EntityId createDefaultShip(vector3df position, vector3df rotation);

//Creates a default AI ship. Includes AI, ship, bullet, irrlicht, weapon, sensors, faction, and health components. Returns the ID.
EntityId createDefaultAIShip(vector3df position, vector3df rotation);

//Adds on the ship's collision body (i.e., the rigid body component) to the entity.
bool initializeShipCollisionBody(EntityId entityId, u32 shipId, bool carrier=false);

//Initializes a weapon component on the given ship at the given hardpoint from the ID given.
//Example: The "Plasma Blaster" has the weapon id 1. Pass in 1 to this function to load a Plasma Blaster onto the hardpoint.
//If you want to load a physics weapon set the bool to "true".
bool initializeWeaponFromId(u32 id, EntityId shipId, int hardpoint, bool phys=false);
//Adds a default weapon to the given ship at the given hardpoint.
//Requires: Irrlicht component, ship component. Returns false without that.
bool initializeDefaultWeapon(EntityId shipId, int hardpoint);

//Creates a faction component on the entity with the given hostilities, friendlies, and type.
void initializeFaction(EntityId id, FACTION_TYPE type, u32 hostiles, u32 friendlies);
//Creates a faction component on the given entity set to neutral.
void initializeNeutralFaction(EntityId id);
//Creates a faction component on the given entity set to hostile.
void initializeHostileFaction(EntityId id);
//Creates a faction component on the given entity set to the player faction.
void initializePlayerFaction(EntityId id);
//Changes the faction type and hostilities/friendlies of the given faction. Allegiances should be passed in as a bitmask.
void setFaction(FactionComponent* fac, FACTION_TYPE type, unsigned int hostilities, unsigned int friendlies);

//Adds sensors to the given entity. Requires a bullet and a faction component.
bool initializeSensors(EntityId id, f32 range, f32 updateInterval);
//Adds sensors to the given entity with default values. Requires a bullet component and a faction component.
bool initializeDefaultSensors(EntityId id);

//Adds shield component to the given entity.
void initializeShields(EntityId id, f32 amount, f32 delay, f32 recharge);
//Adds default shield values.
void initializeDefaultShields(EntityId objectId);

//Creates a particle jet for the ship at the given position.
IParticleSystemSceneNode* createShipJet(ISceneNode* node, vector3df pos, vector3df dir);
//Adds the particle system to a given ship.
void initializeShipParticles(EntityId id);

EntityId createShipFromInstance(ShipInstance& inst, vector3df pos, vector3df rot);
EntityId createPlayerShipFromInstance(vector3df pos, vector3df rot);
#endif 
