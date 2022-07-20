#pragma once

#ifndef SHIPCOMPONENT_H
#define SHIPCOMPONENT_H
#include "BaseHeader.h"
#include "Shaders.h"
#include <vector>

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

	f32 afterburnerFuel;
	f32 maxAfterburnerFuel;
	f32 afterburnerFuelEfficiency;
	bool afterburnerOn;

	bool safetyOverride = true;

	vector3df upJetPos[2];
	vector3df downJetPos[2];
	vector3df leftJetPos[2];
	vector3df rightJetPos[2];
	vector3df reverseJetPos[2];
	vector3df engineJetPos;
};

#endif