#include "SceneManager.h"
#include "GameController.h"


void SceneManager::update(f32 time, f32 frameDelta)
{
	dt = time;
	parentUpdate(sceneManager->scene); //checks to see whether parents are currently in use
	soundSystem(dt); //moves around sounds and tracks sound instances
	shipControlSystem(dt); //updates ship control scheme
	shipUpdateSystem(dt); //applies forces, applies particle effects on ships
	AIUpdateSystem(dt); //updates AI components
	weaponFiringSystem(dt); //fires guns
	bWorld->stepSimulation(dt, 60); //applies the PHYSICS
	collisionCheckingSystem(); //destroys projectiles on hit
	damageSystem(dt); //Updates damage tracking components
	updateHealthSystem(); //updates health and removes objects with no health
	updateShieldSystem(dt); //updates shields and recharging thereof
	explosionSystem(dt); //Handles explosion lifetimes
	irrlichtRigidBodyPositionSystem(sceneManager->scene, dt); //updates position based on rigidbody state
	projectileSystem(dt); //kills projectiles after they have gone a set distance
	playerUpdateSystem(sceneManager->scene, frameDelta); //updates camera location
	sensorSystem(dt); //updates sensors and nearby active contacts
}

