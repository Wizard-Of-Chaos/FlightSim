#pragma once
#ifndef SENSORCOMPONENT_H
#define SENSORCOMPONENT_H
#include "BaseHeader.h"
#include "ECS.h"
#include "ShipComponent.h"
#include "IrrlichtComponent.h"
#include "FactionComponent.h"

//Default range of a sensor
#define DEFAULT_SENSOR_RANGE 500.f

/*
* The sensor component allows a given entity to "see" what's around it. It looks for any Irrlicht node
* within the radius (default 500), checks whether or not it's friendly, and then updates its list of contacts
* accordingly (as well as closest hostile and friendly contacts).
* 
* This component is updated in the SensorUpdateSystem.
*/
struct SensorComponent
{
	f32 detectionRadius;
	array<EntityId> contacts;
	EntityId closestContact;
	EntityId closestHostileContact;
	EntityId closestFriendlyContact;

	EntityId targetContact; //contact for whoever the sensors are actually focused on
	f32 timeSelected;
};

#endif