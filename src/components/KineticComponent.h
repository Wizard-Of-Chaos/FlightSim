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
	f32 reloadTime;
};

#endif 