#pragma once
#ifndef SENSORUPDATESYSTEM_H
#define SENSORUPDATESYSTEM_H

#include "BaseHeader.h"
#include "ECS.h"
#include "SensorComponent.h"
#include "ShipComponent.h"
#include "IrrlichtComponent.h"
#include "BulletRigidBodyComponent.h"

class SceneManager;
class GameController;

/*
* Updates all sensor components, which are used by the AI and by the player to determine what the hell is going on.
* It checks whether or not an entity is in range of the sensor, and updates accordingly.
*/
std::vector<ContactInfo> getContacts(SceneManager* manager, BulletRigidBodyComponent* rbc, SensorComponent* sens, FactionComponent* fac);
void sensorSystem(SceneManager* manager, f32 dt);

#endif 