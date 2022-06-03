#pragma once
#ifndef BOLASCOMPONENT_H
#define BOLASCOMPONENT_H
#include "BaseHeader.h"

struct BolasInfoComponent
{
	flecs::entity target1;
	flecs::entity target2;
	f32 duration;
	f32 currentDuration;
	f32 timeToHit; //the time required between two hits
	f32 currentTimeToHit;
	f32 force;
	btTypedConstraint* constraint;
};

#endif 