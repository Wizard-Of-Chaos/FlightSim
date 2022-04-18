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
#include "ExplosionSystem.h"
#include "HealthAndShieldSystems.h"
#include "SoundSystem.h"
#include "DamageSystem.h"
/*
* The poorly-named SceneManager (seriously, Irrlicht already took that name!) manages the ECS scene. It calls the various systems
* to update all the components in the scene, as well as holds the default textures and such for the scene. Update is called from the main
* loop in the GameController class.
*/
class SceneManager {
	public:
		SceneManager(Scene scene) : scene(scene) {}
		SceneManager() {} 
		void update(f32 dt, f32 frameDelta);
		f32 dt = 0;
		Scene scene;
};

#endif