#pragma once

#ifndef SHIPCOMPONENT_H
#define SHIPCOMPONENT_H
#include "BaseHeader.h"
#include "ECS.h"
#include <vector>

/*
* The different types of movement a ship can make.
* Thrust forward, strafe left/right/up/down/back, and then pitch/yaw/roll.
* It also has STOP_VELOCITY and STOP_ROTATION to get the opposing force to rotation and velocity.
*
* The SHIP_MAX_MOVEMENTS enum is used to create arrays for ship movement.
*/
enum SHIP_MOVEMENT {
	SHIP_THRUST_FORWARD,
	SHIP_AFTERBURNER,
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

//The maximum amount of weapons any ship can have.
const int MAX_HARDPOINTS = 8;

//Default thrust capacity for a ship.
const f32 DEFAULT_FORWARD_THRUST = 75.f;
const f32 DEFAULT_BRAKE_THRUST = 45.f;
const f32 DEFAULT_STRAFE_THRUST = 20.f;
const f32 DEFAULT_PITCH_THRUST = 90.f;
const f32 DEFAULT_YAW_THRUST = 80.f;
const f32 DEFAULT_ROLL_THRUST = 45.f;
const f32 DEFAULT_AFTERBURNER_THRUST = 50.f;
const f32 DEFAULT_VELOCITY_TOLERANCE = 1.f;
const f32 DEFAULT_BASE_MAX_VELOCITY = 100.f;
const f32 DEFAULT_TOTAL_MAX_VELOCITY = 150.f;
const f32 DEFAULT_MAX_ANGULAR_VELOCITY = 1.f; //radians
const f32 DEFAULT_AFTERBURNER_FUEL = 100.f;
const f32 DEFAULT_AFTERBURNER_FUEL_EFFICIENCY = 2.f;

/*
* The ship component holds a lot of information about what a ship is.
* It holds the amount of hardpoints, a list of positions for those hardpoints, the weapon ID for those hardpoints,
* and the thrusts for each way the ship can move.
* 
* The thrusts are self explanatory. curPitch and curYaw are used in combination with mouse movement, to represent
* where the mouse is on the screen.
* 
* The "moves" array holds which movements are currently triggered.
* 
* Finally, the vectors and emitters hold the particle effects for the different thrusts a ship can do.
*/
struct ShipComponent {
	u32 hardpointCount;
	//This and the weapons array are initialized to the maximum of 8.
	vector3df hardpoints[MAX_HARDPOINTS];
	EntityId weapons[MAX_HARDPOINTS];

	f32 forwardThrust;
	f32 brakeThrust;
	f32 strafeThrust;

	f32 pitchThrust;
	f32 yawThrust;
	f32 rollThrust;

	f32 curPitch;
	f32 curYaw;

	f32 afterburnerFuel;
	f32 afterburnerFuelEfficiency;
	//Holds the movements that the ship is currently trying to make (pitch, yaw, thrust, etc).
	bool moves[SHIP_MAX_MOVEMENTS];

	//Positions on the ship for where the thrust emissions come from.
	vector3df upJetPos[2];
	IParticleSystemSceneNode* upJetEmit[2];
	vector3df downJetPos[2];
	IParticleSystemSceneNode* downJetEmit[2];
	vector3df leftJetPos[2];
	IParticleSystemSceneNode* leftJetEmit[2];
	vector3df rightJetPos[2];
	IParticleSystemSceneNode* rightJetEmit[2];
	vector3df reverseJetPos[2];
	IParticleSystemSceneNode* reverseJetEmit[2];
	vector3df engineJetPos;
	IParticleSystemSceneNode* engineJetEmit;
	ILightSceneNode* engineLight; //The engine light is on! Check your oil.

	bool afterburnerOn;
	bool safetyOverride;
	f32 velocityTolerance; //how tolerant the ship is of going faster than its expected to; damage multiplied by this factor
	f32 linearMaxVelocity; //the max velocity a ship can handle without taking damage
	f32 angularMaxVelocity; //max rotational speed
	f32 afterburnerThrust; //how much thrust the afterburner can supply
};

#endif