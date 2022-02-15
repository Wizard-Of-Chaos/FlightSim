#pragma once
#ifndef SENSORCOMPONENT_H
#define SENSORCOMPONENT_H
#include "BaseHeader.h"
#include "ECS.h"
#include "ShipComponent.h"
#include "IrrlichtComponent.h"
#include "FactionComponent.h"

#define DEFAULT_SENSOR_RANGE 500.f

struct SensorComponent
{
	f32 detectionRadius;
	array<EntityId> contacts;
	EntityId closestContact;
	EntityId closestHostileContact;
	EntityId closestFriendlyContact;
};

#endif