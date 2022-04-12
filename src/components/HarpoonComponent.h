#pragma once
#ifndef HARPOONCOMPONENT_H
#define HARPOONCOMPONENT_H
#include "BaseHeader.h"
#include "ECS.h"

struct HarpoonInfoComponent
{
	EntityId target;
	f32 duration;
	f32 maxLength;
};

struct HarpoonProjectileComponent
{
	EntityId target;
	f32 currentDuration;
	bool latched;
};
#endif 