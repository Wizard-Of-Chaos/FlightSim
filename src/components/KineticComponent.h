#pragma once
#ifndef KINETICCOMPONENT_H
#define KINETICCOMPONENT_H
#include "BaseHeader.h"
#include "ECS.h"

struct KineticInfoComponent
{
	u32 maxAmmunition;
	u32 ammunition;
	u32 clip;
	u32 maxClip;
	f32 timeReloading;
	f32 reloadTime;
};

#endif 