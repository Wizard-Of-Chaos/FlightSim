#pragma once

#ifndef AICOMPONENT_H
#define AICOMPONENT_H
#include "BaseHeader.h"

/*
* The different states an AI might be in. Implemented using steering behaviors in
* the AI_Systems folder.
*/
enum AI_STATE
{
	AI_STATE_IDLE,
	AI_STATE_PURSUIT,
	AI_STATE_FLEE,
	AI_STATE_SEARCH,
	AI_STATE_PATROL,
	AI_STATE_FORM_ON_WING
};

enum AI_TYPE //different ships would have different AI patterns
{
	AI_TYPE_DEFAULT
};

//Default reaction time. Default is to check state once per second.
const f32 AI_DEFAULT_REACTION_TIME = 1.f;
//The AI will run when it is below 25% health, by default. Represented as a float.
const f32 AI_DEFAULT_DAMAGE_TOLERANCE = .4f;

//The component that controls the AI for a given AI ship.
//Includes the type of AI, the current state of the AI, how quickly it reacts
//(changes state), the damage point at which it will flip out, and time since
//the last time it changed states (internal use).
struct AIComponent
{
	AI_TYPE AIType;
	AI_STATE state;
	f32 reactionSpeed; //how long it takes to change states
	f32 timeSinceLastStateCheck; //this is going to be the time since the last state check

	f32 damageTolerance; // %hp that the AI will run at
};

#endif
