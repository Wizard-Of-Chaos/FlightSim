#pragma once

#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H
#include "BaseHeader.h"
#include "ECS.h"

const f32 DEFAULT_MAX_HEALTH = 100.f;

struct HealthComponent
{
	f32 health;
	f32 maxHealth;
};

#endif