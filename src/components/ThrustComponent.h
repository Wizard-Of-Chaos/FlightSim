#pragma once
#ifndef THRUSTCOMPONENT_H
#define THRUSTCOMPONENT_H
#include "BaseHeader.h"

/*
* The different types of movement a thrustable object can make.
* Thrust forward, strafe left/right/up/down/back, and then pitch/yaw/roll.
* It also has STOP_VELOCITY and STOP_ROTATION to get the opposing force to rotation and velocity.
*
* The MAX_MOVEMENTS enum is used to create arrays for movement.
*/

enum MOVEMENT {
	THRUST_FORWARD,
	THRUST_BOOST,
	THRUST_BACKWARD,
	STRAFE_LEFT,
	STRAFE_RIGHT,
	STRAFE_UP,
	STRAFE_DOWN,
	PITCH_UP,
	PITCH_DOWN,
	ROLL_LEFT,
	ROLL_RIGHT,
	YAW_LEFT,
	YAW_RIGHT,
	STOP_VELOCITY,
	STOP_ROTATION,
	MAX_MOVEMENTS
};

struct ThrustComponent
{
	f32 pitch=0.f;
	f32 yaw=0.f;
	f32 roll=0.f;
	f32 forward=0.f;
	f32 strafe=0.f;
	f32 brake=0.f;
	f32 boost=0.f; //if the thing has a boost capability (afterburners).
	bool moves[MAX_MOVEMENTS];
	f32 velocityTolerance=0.0001f; //How tolerant the entity is of going over the max speed it has
	f32 linearMaxVelocity=1000.f; //Max linear velocity
	f32 angularMaxVelocity=1000.f; //Max angular velocity

	ThrustComponent() {
		for (u32 i = 0; i < MAX_MOVEMENTS; ++i) {
			moves[i] = false;
		}
	}
};
#endif 