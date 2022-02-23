#pragma once

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include "BaseHeader.h"

#include "ECS.h"
#include "ShipControlSystem.h"
#include "IrrlichtRigidBodyPositionSystem.h"
#include "PlayerUpdateSystem.h"
#include "WeaponFiringSystem.h"
#include "CollisionCheckingSystem.h"
#include "SensorUpdateSystem.h"
#include "ShipUpdateSystem.h"
#include "AIUpdateSystem.h"
#include "HealthSystem.h"
#include "GameDefaults.h"

class GameController;

class SceneManager {
	public:
		SceneManager(Scene scene, GameController* controller, btDiscreteDynamicsWorld* bulletWorld) : controller(controller), scene(scene), bulletWorld(bulletWorld) {}
		SceneManager() : controller(0), bulletWorld(0) {} 
		void update(f32 dt, f32 frameDelta);
		f32 dt = 0;
		Scene scene;
		GameController* controller;
		btDiscreteDynamicsWorld* bulletWorld;
		GameDefaults defaults;
};

#endif