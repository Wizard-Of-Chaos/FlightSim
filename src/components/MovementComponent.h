#pragma once
#ifndef MOVEMENTCOMPONENT_H
#define MOVEMENTCOMPONENT_H
#include "BaseHeader.h"

enum MOVEMENT {
	THRUST_FORWARD,
	AFTERBURNER,
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
	f32 pitch;
	f32 yaw;
	f32 roll;
	f32 forward;
	f32 strafe;
	f32 brake;
	bool moves[MAX_MOVEMENTS];
	f32 velocityTolerance;
	f32 linearMaxVelocity;
	f32 angularMaxVelocity;
};
#endif 