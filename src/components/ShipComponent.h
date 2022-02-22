#pragma once

#ifndef SHIPCOMPONENT_H
#define SHIPCOMPONENT_H
#include "BaseHeader.h"
#include "ECS.h"
#include <vector>

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

	vector3df upJetPos[2];
	IParticleEmitter* upJetEmit[2];
	vector3df downJetPos[2];
	IParticleEmitter* downJetEmit[2];
	vector3df leftJetPos[2];
	IParticleEmitter* leftJetEmit[2];
	vector3df rightJetPos[2];
	IParticleEmitter* rightJetEmit[2];

};

#endif