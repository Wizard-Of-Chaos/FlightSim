#pragma once

#ifndef SHIPMOVEMENTSYSTEM_H
#define SHIPMOVEMENTSYSTEM_H
#include "BaseHeader.h"

#include "ECS.h"
#include "InputComponent.h"
#include "IrrlichtComponent.h"
#include "BulletRigidBodyComponent.h"
#include "ShipMovementUtils.h"
#include "ShipComponent.h"

/*
* The way that the actual buttons on the keyboard talk to the player entity. This function updates the ship component
* to pitch, yaw, and roll according to keyboard inputs.
* 
* TODO: Abstract keyboard inputs to be mapped to player-set keybinds.
*/
void shipControlSystem(Scene& scene, f32 dt);

#endif