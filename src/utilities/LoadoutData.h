#pragma once
#include "BaseHeader.h"
#include "ECS.h"
#include "ShipComponent.h"
#include "WeaponInfoComponent.h"
#include "MissileComponent.h"

/*
* Contains all data necessary to be able to properly load a ship in the game.
* The u32 id is NOT an entity id.
*/
struct ShipData
{
	u32 id;
	std::string name;
	std::string description;
	ShipComponent shipComponent;
	IMesh* shipMesh;
	ITexture* shipTexture;
	ITexture* engineTexture;
	ITexture* jetTexture;
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
	IMesh* weaponMesh;
	ITexture* weaponTexture;
	ITexture* weaponEffect;
};

/*
* An extension to the base WeaponData class that also contains a mesh for the missile model
* and the missile component associated with it.
*/
struct MissileData : public WeaponData
{
	MissileInfoComponent missileComponent;
	IMesh* missileMesh;
};