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
	vector3df axis = shipForward.crossProduct(targetForward);
	f32 maxAngle = 20.f * DEGTORAD;

	quaternion nodeRot(playerShip->getRotation() * DEGTORAD);
	quaternion targetRot(targetnode->getRotation() * DEGTORAD);

	quaternion desiredRot;
	vector3df targetRotVec(0, 0, 0);
	desiredRot.slerp(targetRot, nodeRot, player->slerpFactor*(abs(angle)*10.f));
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
	ICameraSceneNode* camera = player->camera;

	ISceneCollisionManager* coll = manager->controller->smgr->getSceneCollisionManager();
	line3df ray = coll->getRayFromScreenCoordinates(input->mousePixPosition, camera);

	vector3df aheadCrosshairPos = playerShip->getPosition() + getNodeForward(playerShip) * 300.f;
	position2di crosshairPos = coll->getScreenCoordinatesFrom3DPosition(aheadCrosshairPos, camera);
	crosshairPos.X -= 64;
	crosshairPos.Y -= 64;
	player->crosshairGui->setRelativePosition(crosshairPos);

	if (input->rightMouseDown) {
		ISceneNode* selection = coll->getSceneNodeFromRayBB(ray, ID_IsSelectable);
		if (selection) {
			if (selection->getID() != -1) {
				player->activeSelection = selection;
				player->selectionGui->setVisible(true);
				position2di selectionPos = coll->getScreenCoordinatesFrom3DPosition(selection->getAbsolutePosition(), camera);
				selectionPos.X -= 64;
				selectionPos.Y -= 64;
				player->selectionGui->setRelativePosition(selectionPos);
			}
		}
		else if (!selection) {
			player->selectionGui->setVisible(false);
			player->activeSelection = nullptr;
		}
	}
	if (player->activeSelection) {
		position2di selectionPos = coll->getScreenCoordinatesFrom3DPosition(player->activeSelection->getAbsolutePosition(), camera);
		selectionPos.X -= 64;
		selectionPos.Y -= 64;
		player->selectionGui->setRelativePosition(selectionPos);
	}

}