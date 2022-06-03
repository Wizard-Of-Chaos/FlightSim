#pragma once

#ifndef SHIPCOMPONENT_H
#define SHIPCOMPONENT_H
#include "BaseHeader.h"
#include "Shaders.h"
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
	SHIP_THRUST_BACKWARD,
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
const u32 MAX_HARDPOINTS = 6;
#define PHYS_HARDPOINT MAX_HARDPOINTS +1

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
	u32 shipDataId;
	u32 hardpointCount;
	//This and the weapons array are initialized to the maximum of 8.
	vector3df hardpoints[MAX_HARDPOINTS];
	flecs::entity weapons[MAX_HARDPOINTS];

	vector3df physWeaponHardpoint;
	flecs::entity physWeapon;

	f32 forwardThrust;
	f32 brakeThrust;
	f32 strafeThrust;

	f32 pitchThrust;
	f32 yawThrust;
	f32 rollThrust;

	f32 afterburnerFuel;
	f32 maxAfterburnerFuel;
	f32 afterburnerFuelEfficiency;
	//Holds the movements that the ship is currently trying to make (pitch, yaw, thrust, etc).
	bool moves[SHIP_MAX_MOVEMENTS];

	bool afterburnerOn;
	bool safetyOverride;
	f32 velocityTolerance; //how tolerant the ship is of going faster than its expected to; damage multiplied by this factor
	f32 linearMaxVelocity; //the max velocity a ship can handle without taking damage
	f32 angularMaxVelocity; //max rotational speed
	f32 afterburnerThrust; //how much thrust the afterburner can supply

	vector3df upJetPos[2];
	vector3df downJetPos[2];
	vector3df leftJetPos[2];
	vector3df rightJetPos[2];
	vector3df reverseJetPos[2];
	vector3df engineJetPos;
};

#endif