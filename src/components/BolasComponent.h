#pragma once
#ifndef BOLASCOMPONENT_H
#define BOLASCOMPONENT_H
#include "BaseHeader.h"
#include "ECS.h"

struct BolasInfoComponent
{
	EntityId target1;
	EntityId target2;
	f32 duration;
	f32 currentDuration;
	f32 timeToHit; //the time required between two hits
	f32 force;
	btPoint2PointConstraint* constraint;
};

#endif 