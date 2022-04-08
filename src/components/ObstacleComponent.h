#pragma once
#ifndef OBSTACLE_COMPONENT_H
#define OBSTACLE_COMPONENT_H
#include "BaseHeader.h"

enum class OBSTACLE
{
	ASTEROID,
	GAS_CLOUD,
	SPACE_STATION,
	DEBRIS
};

struct ObstacleComponent
{
	OBSTACLE type;
};

#endif 