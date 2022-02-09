#pragma once

#ifndef AICOMPONENT_H
#define AICOMPONENT_H
#include "BaseHeader.h"
#include "ECS.h"

const enum AI_STATE
{
	AI_STATE_IDLE,
	AI_STATE_PURSUIT,
	AI_STATE_FLEE,
	AI_STATE_SEARCH,
	AI_STATE_GET_HELP,
	AI_STATE_PATROL,
	AI_STATE_FORM_ON_WING
};

const enum AI_TYPE //different ships would have different AI patternst
{
	AI_TYPE_DEFAULT
};

const f32 AI_DEFAULT_REACTION_TIME = 1.f;
const f32 AI_DEFAULT_DETECTION_RADIUS = 400.f;

struct AIComponent
{
	AI_TYPE AIType;
	AI_STATE state;
	f32 reactionSpeed; //how long it takes to change states
	f32 timeSinceLastStateCheck; //this is going to be the time since the last state check
	f32 detectionRadius;
	array<EntityId> contacts;
	EntityId closestContact;
	bool nearbyHostiles;
};

#endif