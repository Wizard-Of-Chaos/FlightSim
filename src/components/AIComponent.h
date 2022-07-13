#pragma once

#ifndef AICOMPONENT_H
#define AICOMPONENT_H
#include "BaseHeader.h"
#include <unordered_map>
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
	AI_TYPE_DEFAULT,
	AI_TYPE_ACE //aces leading squadrons
};

const std::unordered_map<std::string, AI_TYPE> aiTypeStrings = {
	{"default", AI_TYPE_DEFAULT},
	{"ace", AI_TYPE_ACE}
};

//Default reaction time. Default is to check state once per second.
const f32 AI_DEFAULT_REACTION_TIME = 1.f;
//The AI will run when it is below 25% health, by default. Represented as a float.
const f32 AI_DEFAULT_COWARDICE = .4f;
//When issued a form-on-wing order the AI will try to get 25 units away.
const f32 AI_DISTANCE_FROM_WING = 25.f;
//If the AI gets further away than the max distance it will regroup with its buddies first.
const f32 AI_MAX_DISTANCE_FROM_WING = 200.f;
//The multiplier on how far the AI will go outside of their wing chasing an enemy.
const f32 AI_DEFAULT_AGGRESSIVENESS = 1.f;
//How well the AI aims, where 1 is perfect aim and 0 is entirely random.
const f32 AI_DEFAULT_AIM = 1.f;

class AIType;

//The component that controls the AI for a given AI ship.
//Includes the type of AI, the current state of the AI, how quickly it reacts
//(changes state), the damage point at which it will flip out, and time since
//the last time it changed states (internal use).
struct AIComponent
{
	AI_TYPE type;
	AIType* aiControls;
	AI_STATE state;
	f32 reactionSpeed = AI_DEFAULT_REACTION_TIME; //how long it takes to change states
	f32 timeSinceLastStateCheck = 0; //this is going to be the time since the last state check
	f32 timeInCurrentState = 0;

	f32 cowardice = AI_DEFAULT_COWARDICE; // %hp that the AI will run at
	f32 aggressiveness = AI_DEFAULT_AGGRESSIVENESS; //multiplier on how far the AI will go on a chase
	f32 aim = AI_DEFAULT_AIM; //how well the AI aims

	f32 wingDistance = AI_DISTANCE_FROM_WING;
	f32 maxWingDistance = AI_MAX_DISTANCE_FROM_WING;

	flecs::entity wingCommander; //if the AI isn't an ace
};
#endif
