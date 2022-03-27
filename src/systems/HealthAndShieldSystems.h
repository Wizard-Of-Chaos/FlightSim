#pragma once
#ifndef HEALTHANDSHIELDSYSTEMS_H
#define HEALTHANDSHIELDSYSTEMS_H
#include "BaseHeader.h"
#include "ECS.h"
#include "IrrlichtComponent.h"
#include "BulletRigidBodyComponent.h"
#include "HealthComponent.h"
#include "GameFunctions.h"

class SceneManager;
class GameStateController;
class GameController;

//Updates health for all entities in the scene. If health is 0, removes it from the scene.
void updateHealthSystem(SceneManager* manager);

//Updates shield values for all entities in the scene.
void updateShieldSystem(SceneManager* manager, f32 dt);

#endif 

