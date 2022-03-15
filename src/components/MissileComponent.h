#pragma once
#ifndef MISSILECOMPONENT_H
#define MISSILECOMPONENT_H
#include "BaseHeader.h"
#include "ECS.h"

struct MissileInfoComponent
{
	f32 missileRotThrust;
	f32 maxMissileVelocity;
};

struct MissileProjectileComponent
{
	EntityId target;
	//effectively a stripped-down ship component here
	f32 rotThrust;
	f32 maxVelocity;
};

#endif 