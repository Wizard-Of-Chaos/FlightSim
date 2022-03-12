#pragma once
#ifndef MISSILECOMPONENT_H
#define MISSILECOMPONENT_H
#include "BaseHeader.h"
#include "ECS.h"

struct MissileComponent
{
	EntityId target;
};

#endif 