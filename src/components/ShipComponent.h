#pragma once

#ifndef SHIPCOMPONENT_H
#define SHIPCOMPONENT_H
#include "BaseHeader.h"
#include "ECS.h"
#include <vector>

enum SHIP_MOVEMENT {
	SHIP_THRUST_FORWARD,
	SHIP_STRAFE_BACKWARD,
	SHIP_STRAFE_LEFT,
	SHIP_STRAFE_RIGHT,
	SHIP_STRAFE_UP,
	SHIP_STRAFE_DOWN,
	SHIP_PITCH_UP,
	SHIP_PITCH_DOWN,
	SHIP_ROLL_LEFT,
	SHIP_ROLL_RIGHT,
	SHIP_YAW_LEFT,
	SHIP_YAW_RIGHT,
	SHIP_STOP_VELOCITY,
	SHIP_STOP_ROTATION,
	SHIP_MAX_MOVEMENTS
};

const int MAX_HARDPOINTS = 8;

const f32 DEFAULT_FORWARD_THRUST = 75.f;
const f32 DEFAULT_BRAKE_THRUST = 45.f;
const f32 DEFAULT_STRAFE_THRUST = 20.f;
const f32 DEFAULT_PITCH_THRUST = 90.f;
const f32 DEFAULT_YAW_THRUST = 80.f;
const f32 DEFAULT_ROLL_THRUST = 45.f;

struct ShipComponent {
	unsigned int hardpointCount;
	vector3df hardpoints[MAX_HARDPOINTS];
	EntityId weapons[MAX_HARDPOINTS];

	f32 forwardThrust;
	f32 brakeThrust;
	f32 strafeThrust;

	f32 pitchThrust;
	f32 yawThrust;
	f32 rollThrust;

	bool moves[SHIP_MAX_MOVEMENTS];

	vector3df upJetPos[2];
	IParticleSystemSceneNode* upJetEmit[2];
	vector3df downJetPos[2];
	IParticleSystemSceneNode* downJetEmit[2];
	vector3df leftJetPos[2];
	IParticleSystemSceneNode* leftJetEmit[2];
	vector3df rightJetPos[2];
	IParticleSystemSceneNode* rightJetEmit[2];

};

#endif