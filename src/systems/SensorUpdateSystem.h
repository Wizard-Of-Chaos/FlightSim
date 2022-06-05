#pragma once
#ifndef SENSORUPDATESYSTEM_H
#define SENSORUPDATESYSTEM_H

#include "BaseHeader.h"

#include "SensorComponent.h"
#include "ShipComponent.h"
#include "IrrlichtComponent.h"
#include "BulletRigidBodyComponent.h"

/*
* Updates all sensor components, which are used by the AI and by the player to determine what the hell is going on.
* It checks whether or not an entity is in range of the sensor, and updates accordingly.
*/
std::vector<ContactInfo> getContacts(BulletRigidBodyComponent* rbc, SensorComponent* sens, FactionComponent* fac);
void sensorSystem(flecs::iter it, BulletRigidBodyComponent* rbcs, SensorComponent* sns, FactionComponent* fcs);


#endif 