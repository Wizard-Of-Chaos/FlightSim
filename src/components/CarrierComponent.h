#pragma once
#ifndef CARRIERCOMPONENT_H
#define CARRIERCOMPONENT_H
#include "BaseHeader.h"
#include "ShipInstance.h"

const u32 CARRIER_MAX_SHIP_TYPES = 4;
const u32 CARRIER_MAX_TURRETS = 8;

struct CarrierComponent
{
	f32 spawnRate;
	u32 shipTypeCount;
	ShipInstance spawnShipTypes[CARRIER_MAX_SHIP_TYPES];
	u32 reserveShips;
	vector3df scale;
	f32 spawnTimer;

	flecs::entity turrets[CARRIER_MAX_TURRETS];
	vector3df turretPositions[CARRIER_MAX_TURRETS];
	vector3df turretRotations[CARRIER_MAX_TURRETS];
};

#endif 