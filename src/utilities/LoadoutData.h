#pragma once
#include "BaseHeader.h"
#include "ECS.h"
#include "ShipComponent.h"
#include "WeaponInfoComponent.h"
#include "MissileComponent.h"

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

struct MissileData : public WeaponData
{
	MissileInfoComponent missileComponent;
	IMesh* missileMesh;
};