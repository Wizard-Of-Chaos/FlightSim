#include "SceneManager.h"
#include "Controller.h"


void SceneManager::update(f32 time)
{
	dt = time;
	shipMovementSystem(scene, dt); //updates impulses based on player input
	integratePhysicsSystem(scene, dt); //applies the PHYSICS
	irrlichtRigidBodyPositionSystem(scene, dt); //updates position based on rigidbody state
	playerUpdateSystem(scene, dt); //updates camera location
}

