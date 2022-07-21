#pragma once
#include "BaseHeader.h"
#include "ShipComponent.h"
#include "WeaponInfoComponent.h"
#include "HealthComponent.h"
#include "MissileComponent.h"
#include "ObstacleComponent.h"
#include "KineticComponent.h"
#include "CarrierComponent.h"
#include "BolasComponent.h"
#include "AIComponent.h"
#include "ThrustComponent.h"
#include "TurretComponent.h"

/*
* Contains all data necessary to be able to properly load a ship in the game.
* The u32 id is NOT an entity id.
* TODO: Remove this struct entirely and switch to ship instances.
*/
struct ShipData
{
	u32 id;
	std::string name;
	std::string description;
	ShipComponent shipComponent;
	ThrustComponent thrustComponent;
	btConvexHullShape collisionShape;

	std::string shipMesh;
	std::string shipTexture;
	std::string engineTexture;
	std::string jetTexture;
	std::string shipNorm;
};

struct CarrierData : public ShipData
{
	CarrierComponent carrierComponent;
	u32 mass;
	vector3df scale;
	f32 health;
};

struct TurretData
{
	u32 id;
	TurretComponent turretComponent;
	ThrustComponent thrustComponent;

	std::string name;
	std::string description;

	std::string mesh;
	std::string texture;
	std::string norm;
};
/*
* Contains all baseline data for a given weapon. The u32 id is NOT an entity id.
* Take note of additional subclasses when loading and constructing new weapon types.
*/
struct WeaponData
{
	u32 id;
	std::string name;
	std::string description;
	WeaponInfoComponent weaponComponent;

	std::string weaponMesh;
	std::string weaponTexture;
	std::string weaponEffect;
	std::string weaponNorm;
	std::string weaponFireSound;
};

/*
* An extension to the base WeaponData class that also contains a mesh for the missile model
* and the missile component associated with it.
*/
struct MissileData : public WeaponData
{
	MissileInfoComponent missileComponent;
	std::string missileMesh;
	std::string missileTexture;
};

struct KineticData : public WeaponData
{
	KineticInfoComponent kineticComponent;
};

struct BolasData : public WeaponData
{
	BolasInfoComponent bolasComponent;
};

struct WingmanData
{
	u32 id; //0 is the player
	std::string name;
	std::string description;
	AIComponent ai;
	ShipInstance* assignedShip = nullptr;
	u32 totalKills = 0;
	u32 totalInjuries = 0;
	bool injured = false;
	bool assigned = false;
};

/*
* All the data necessary to load an obstacle, including type, id, mesh, texture, and shape.
*/
struct ObstacleData
{
	u32 id;
	std::string obstacleMesh;
	std::string obstacleTexture;
	std::string obstacleNorm;
	std::string name;
	btConvexHullShape shape;
	OBSTACLE type;
	f32 health;
};