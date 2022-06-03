#pragma once

#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H
#include "BaseHeader.h"

//Default maximum for health is 100.
const f32 DEFAULT_MAX_HEALTH = 100.f;

/*
* It feels like this should be obvious, but the health component stores the current and max
* health of whatever it's attached to. In the health system, an object at 0 health gets deleted.
*/
struct HealthComponent
{
	f32 health;
	f32 maxHealth;
};

#endif