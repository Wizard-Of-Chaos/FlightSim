#pragma once
#ifndef TURRETCOMPONENT_H
#define TURRETCOMPONENT_H
#include "BaseHeader.h"

const u32 MAX_TURRET_HARDPOINTS = 2;

struct TurretComponent
{
	vector3df startingRotation = vector3df(0,0,0); //this constrains how much the turret will be able to pitch/yaw
	u32 hardpointCount = 0;
	vector3df hardpoints[MAX_TURRET_HARDPOINTS];
	flecs::entity weapons[MAX_TURRET_HARDPOINTS];
};

#endif 