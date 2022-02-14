#include "PlayerUpdateSystem.h"
#include "SceneManager.h"
#include "GameController.h"

#include <iostream>
#include <cmath>

void playerUpdateSystem(SceneManager* manager, Scene& scene, f32 frameDelta)
{
	for (auto entityId : SceneView<IrrlichtComponent, PlayerComponent, BulletRigidBodyComponent, InputComponent>(scene)) {
		IrrlichtComponent* irrcomp = scene.get<IrrlichtComponent>(entityId);
		PlayerComponent* player = scene.get<PlayerComponent>(entityId);
		BulletRigidBodyComponent* rbc = scene.get<BulletRigidBodyComponent>(entityId);
		InputComponent* input = scene.get<InputComponent>(entityId);

		//camera work
		cameraUpdate(player, irrcomp->node, &rbc->rigidBody);

		//HUD work
		hudUpdate(manager, player, irrcomp->node, input);
	}
}

void cameraUpdate(PlayerComponent* player, ISceneNode* playerShip, btRigidBody* body)
{
	ISceneNode* targetnode = player->target;
	ICameraSceneNode* camera = player->camera;
	targetnode->setPosition(playerShip->getPosition());
	vector3df targetForward = getNodeForward(targetnode);
	vector3df shipForward = getNodeForward(playerShip);

	f32 angle = irrVecToBt(targetForward).angle(irrVecToBt(shipForward));
	f32 rollAngle = irrVecToBt(getNodeRight(targetnode)).angle(irrVecToBt(getNodeRight(playerShip)));
	vector3df axis = shipForward.crossProduct(targetForward);

	quaternion nodeRot(playerShip->getRotation() * DEGTORAD);
	quaternion targetRot(targetnode->getRotation() * DEGTORAD);

	f32 slerp = (abs(angle) + abs(rollAngle)) / 2;

	quaternion desiredRot;
	vector3df targetRotVec(0, 0, 0);
	desiredRot.slerp(targetRot, nodeRot, player->slerpFactor*(slerp*10.f));
	desiredRot.toEuler(targetRotVec);
	targetRotVec *= RADTODEG;

	targetnode->setRotation(targetRotVec);

	vector3df targetUp = getNodeUp(targetnode);
	camera->setUpVector(targetUp);
	vector3df target = playerShip->getPosition() + btVecToIrr(body->getLinearVelocity()) * player->velocityFactor;
	camera->setTarget(target);
}

void hudUpdate(SceneManager* manager, PlayerComponent* player, ISceneNode* playerShip, InputComponent* input)
{
	for (HUDElement* elem : player->HUD) {
		elem->updateElement(manager, player, playerShip, input);
	}
}