#pragma once
#ifndef MISSILECOMPONENT_H
#define MISSILECOMPONENT_H
#include "BaseHeader.h"
#include "ECS.h"

struct MissileComponent
{
	EntityId target;
	//effectively a stripped-down ship component here
	f32 rotThrust;
	f32 forwardThrust;
	f32 maxVelocity;
};

#endif 