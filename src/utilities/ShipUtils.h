#pragma once
#ifndef SHIPUTILS_H
#define SHIPUTILS_H
#include "BaseHeader.h"
#include "FactionComponent.h"
#include "LoadoutData.h"
#include "AIComponent.h"
#include "AITypes.h"
#include "ShipParticleComponent.h"

//Creates a ship using the data at the given id in the GameStateController.
//Example: The "Tuxedo" ship has the id 0 in the GameStateController. Throw that ID in here and it will load the ship.
//Adds: ShipComponent, IrrlichtComponent
flecs::entity createShipFromId(u32 id, vector3df position, vector3df rotation);
//Creates a default ship entity at the given position. Does not include hitbox, health, or other components.
//Does include two default weapon entities and an irrlicht component / ship component. Returns the ID.
flecs::entity createDefaultShip(vector3df position, vector3df rotation);

//Creates a default AI ship. Includes AI, ship, bullet, irrlicht, weapon, sensors, faction, and health components. Returns the ID.
flecs::entity createDefaultAIShip(vector3df position, vector3df rotation);

//Creates an ace AI ship. Includes AI, ship, bullet, irrlicht, weapon, sensors, faction, and health components. Returns the ID.
flecs::entity createAceAIShip(vector3df position, vector3df rotation);

//Adds on the ship's collision body (i.e., the rigid body component) to the entity.
bool initializeShipCollisionBody(flecs::entity entityId, u32 shipId, bool carrier=false);

//Initializes a weapon component on the given ship at the given hardpoint from the ID given.
//Example: The "Plasma Blaster" has the weapon id 1. Pass in 1 to this function to load a Plasma Blaster onto the hardpoint.
//If you want to load a physics weapon set the bool to "true".
bool initializeWeaponFromId(u32 id, flecs::entity shipId, int hardpoint, bool phys=false);
//Adds a default weapon to the given ship at the given hardpoint.
//Requires: Irrlicht component, ship component. Returns false without that.
bool initializeDefaultWeapon(flecs::entity shipId, int hardpoint);

//Creates a faction component on the entity with the given hostilities, friendlies, and type.
void initializeFaction(flecs::entity id, FACTION_TYPE type, u32 hostiles, u32 friendlies);
//Creates a faction component on the given entity set to neutral.
void initializeNeutralFaction(flecs::entity id);
//Creates a faction component on the given entity set to hostile.
void initializeHostileFaction(flecs::entity id);
//Creates a faction component on the given entity set to the player faction.
void initializePlayerFaction(flecs::entity id);
//Changes the faction type and hostilities/friendlies of the given faction. Allegiances should be passed in as a bitmask.
void setFaction(FactionComponent* fac, FACTION_TYPE type, unsigned int hostilities, unsigned int friendlies);

//Adds sensors to the given entity. Requires a bullet and a faction component.
bool initializeSensors(flecs::entity id, f32 range, f32 updateInterval);
//Adds sensors to the given entity with default values. Requires a bullet component and a faction component.
bool initializeDefaultSensors(flecs::entity id);

//Adds shield component to the given entity.
void initializeShields(flecs::entity id, f32 amount, f32 delay, f32 recharge);
//Adds default shield values.
void initializeDefaultShields(flecs::entity objectId);

//Creates a particle jet for the ship at the given position.
IParticleSystemSceneNode* createShipJet(ISceneNode* node, vector3df pos, vector3df dir);
//Adds the particle system to a given ship.
void initializeShipParticles(flecs::entity id);

//Builds a ship from the given ship instance with its health, weapons, ship component and all the rest. Returns the ID.
flecs::entity createShipFromInstance(ShipInstance& inst, vector3df pos, vector3df rot);
//Creates a player ship from the current player instance.
flecs::entity createPlayerShipFromInstance(vector3df pos, vector3df rot);
//Creates the given wingman, slots 1-3.
flecs::entity createWingmanFromInstance(u32 num, flecs::entity player, vector3df pos, vector3df rot);

//Creates an AI ship from the given ship instance at the position and rotation specified. Used by carriers.
flecs::entity createAIShipFromInstance(ShipInstance& inst, vector3df pos, vector3df rot);
//Creates an allied AI ship from the given instance.
flecs::entity createFriendlyAIShipFromInstance(ShipInstance& inst, vector3df pos, vector3df rot);
//Creates a hostile AI ship from the given instance.
flecs::entity createHostileAIShipFromInstance(ShipInstance& inst, vector3df pos, vector3df rot);
//Spawns a ship from a carrier and applies the same faction component that the carrier has.
flecs::entity carrierSpawnShip(ShipInstance& inst, vector3df spawnPos, vector3df spawnRot, FactionComponent* carrFac);

//The death callback used by fighters, a loud noise and a small bang.
void fighterDeathExplosionCallback(flecs::entity id);
#endif 
