#pragma once

#ifndef SHIPCONTROLSYSTEM_H
#define SHIPCONTROLSYSTEM_H
#include "BaseHeader.h"

#include "InputComponent.h"
#include "IrrlichtComponent.h"
#include "BulletRigidBodyComponent.h"
#include "ShipMovementUtils.h"
#include "ShipComponent.h"
#include "SensorComponent.h"

/*
* The way that the actual buttons on the keyboard talk to the player entity. This function updates the ship component
* to pitch, yaw, and roll according to keyboard inputs.
* 
* TODO: Abstract keyboard inputs to be mapped to player-set keybinds.
*/
void shipControlSystem(flecs::iter it,
	InputComponent* inc, ShipComponent* shpc, PlayerComponent* plyc, BulletRigidBodyComponent* rbcs, IrrlichtComponent* irrc, SensorComponent* snsc);

#endif