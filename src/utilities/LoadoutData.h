#pragma once
#include "BaseHeader.h"
#include "ECS.h"
#include "ShipComponent.h"
#include "WeaponInfoComponent.h"
#include "MissileComponent.h"
#include "ObstacleComponent.h"
#include "KineticComponent.h"

/*
* Contains all data necessary to be able to properly load a ship in the game.
* The u32 id is NOT an entity id.
*/
struct LoadoutData
{
	u32 shipId;
	u32 weaponIds[MAX_HARDPOINTS];
};

struct ShipData
{
	u32 id;
	std::string name;
	std::string description;
	ShipComponent shipComponent;
	btConvexHullShape collisionShape;

	std::string shipMesh;
	std::string shipTexture;
	std::string engineTexture;
	std::string jetTexture;
	std::string shipNorm;
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