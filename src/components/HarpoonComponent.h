#pragma once
#ifndef HARPOONCOMPONENT_H
#define HARPOONCOMPONENT_H
#include "BaseHeader.h"

struct HarpoonInfoComponent
{
	flecs::entity target;
	f32 duration;
	f32 maxLength;
};

struct HarpoonProjectileComponent
{
	flecs::entity target;
	f32 currentDuration;
	bool latched;
};
#endif 