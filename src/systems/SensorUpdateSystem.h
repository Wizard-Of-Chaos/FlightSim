#pragma once
#ifndef SENSORUPDATESYSTEM_H
#define SENSORUPDATESYSTEM_H

#include "BaseHeader.h"
#include "ECS.h"
#include "SensorComponent.h"
#include "ShipComponent.h"
#include "IrrlichtComponent.h"

class SceneManager;
class GameController;

void sensorUpdateSystem(SceneManager* manager);

#endif 