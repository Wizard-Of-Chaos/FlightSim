#pragma once
#ifndef TURRETCOMPONENT_H
#define TURRETCOMPONENT_H
#include "BaseHeader.h"

const u32 MAX_TURRET_HARDPOINTS = 2;

struct TurretComponent
{
	vector3df startingRotation; //this constrains how much the turret will be able to pitch/yaw
	vector3df hardpoints[MAX_TURRET_HARDPOINTS];
	flecs::entity weapons[MAX_TURRET_HARDPOINTS];
	f32 pitchThrust;
	f32 yawThrust;
};

#endif 