#pragma once
#ifndef DEFAULTAIUPDATESYSTEM_H
#define DEFAULTAIUPDATESYSTEM_H
#include "BaseHeader.h"
#include "AIComponent.h"
#include "SensorComponent.h"
#include "DefaultAIBehaviors.h"

/*
* Includes the default state check for the AI and the update system for the AI.
*/

//The default update system for an AI component.
void defaultAIUpdateSystem(
	AIComponent* ai, IrrlichtComponent* irr, BulletRigidBodyComponent* rbc, ShipComponent* ship, SensorComponent* sensors, HealthComponent* hp,
	f32 dt);
#endif 