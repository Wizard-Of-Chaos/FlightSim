#pragma once
#ifndef OBSTACLE_COMPONENT_H
#define OBSTACLE_COMPONENT_H
#include "BaseHeader.h"

//Different types of obstacle.
enum OBSTACLE
{
	ASTEROID,
	GAS_CLOUD,
	SPACE_STATION,
	DEBRIS,
	EXPLOSIVE_ASTEROID
};

const std::map<std::string, OBSTACLE> obstacleStrings = {
	{"asteroid", ASTEROID},
	{"gascloud", GAS_CLOUD},
	{"spacestation", SPACE_STATION},
	{"debris", DEBRIS},
	{"explosiveasteroid", EXPLOSIVE_ASTEROID}
};

/*
* The obstacle component keeps track of what obstacle type a given obstacle would be. Might do other things later, but for now it's just the one part.
*/
struct ObstacleComponent
{
	OBSTACLE type;
};

#endif 