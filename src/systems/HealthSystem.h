#pragma once
#ifndef HEALTHSYSTEM_H
#define HEALTHSYSTEM_H
#include "BaseHeader.h"
#include "ECS.h"
#include "IrrlichtComponent.h"
#include "BulletRigidBodyComponent.h"
#include "HealthComponent.h"
#include "GameFunctions.h"

class SceneManager;
class GameStateController;
class GameController;

void updateHealthSystem(SceneManager* manager);

#endif 

