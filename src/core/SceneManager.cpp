#include "SceneManager.h"
#include "GameController.h"


void SceneManager::update(f32 time, f32 frameDelta)
{
	dt = time;
	shipControlSystem(scene, dt); //updates ship control scheme
	shipUpdateSystem(scene, dt); //applies forces, applies particle effects on ships
	AIUpdateSystem(this, dt); //updates AI components
	weaponFiringSystem(this, dt); //fires guns
	bulletWorld->stepSimulation(dt, 60); //applies the PHYSICS
	sensorUpdateSystem(this);
	collisionCheckingSystem(this); //destroys projectiles on hit
	updateHealthSystem(this); //updates health and removes objects with no health
	explosionSystem(this, dt); //Handles explosion lifetimes
	irrlichtRigidBodyPositionSystem(scene, dt); //updates position based on rigidbody state
	projectileRangeSystem(this); //kills projectiles after they have gone a set distance
	playerUpdateSystem(this, scene, frameDelta); //updates camera location
}

