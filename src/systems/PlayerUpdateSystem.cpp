#include "PlayerUpdateSystem.h"
#include "SceneManager.h"
#include "GameController.h"
#include "GameStateController.h"

#include <iostream>
#include <cmath>

void playerUpdateSystem(SceneManager* manager, Scene& scene, f32 frameDelta)
{
	for (auto entityId : SceneView<IrrlichtComponent, PlayerComponent, BulletRigidBodyComponent, InputComponent>(scene)) {
		IrrlichtComponent* irrcomp = scene.get<IrrlichtComponent>(entityId);
		PlayerComponent* player = scene.get<PlayerComponent>(entityId);
		BulletRigidBodyComponent* rbc = scene.get<BulletRigidBodyComponent>(entityId);
		InputComponent* input = scene.get<InputComponent>(entityId);
		HealthComponent* hp = scene.get<HealthComponent>(entityId);

		if (input->isKeyDown(KEY_KEY_O)) {
			hp->health -= 1;
		}
		else if (input->isKeyDown(KEY_KEY_P) && hp->health < hp->maxHealth) {
			hp->health += 1;
		}

		SensorComponent* sensors = scene.get<SensorComponent>(entityId);

		for (unsigned int i = 0; i < sensors->contacts.size(); ++i) {
			EntityId contact = sensors->contacts[i];
			if (player->trackedContacts[contact] == nullptr && scene.entityInUse(contact)) {
				HUDContact* elem = new HUDContact(manager, player->rootHUD, contact);
				player->HUD.push_back(elem);
				player->trackedContacts[contact] = elem;
			}
		}

		for (auto [id, hud] : player->trackedContacts) {
			if (!scene.entityInUse(id)) {
				if (hud) player->removeContact(hud);
				continue;
			}
			if (sensors->contacts.binary_search(id) == -1) {
				if (!hud) continue;
				player->removeContact(hud);
			}
		}

		//camera work
		cameraUpdate(player, irrcomp->node, &rbc->rigidBody);

		//HUD work
		hudUpdate(manager, player, entityId);

		manager->controller->soundEngine->setListenerPosition(player->camera->getAbsolutePosition(), getNodeForward(player->camera), vec3df(0, 0, 0), getNodeUp(player->camera));
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

void hudUpdate(SceneManager* manager, PlayerComponent* player, EntityId playerId)
{
	player->rootHUD->setRelativePosition(rect<s32>(position2di(0, 0), manager->controller->driver->getScreenSize()));
	for (HUDElement* elem : player->HUD) {
		elem->updateElement(manager, playerId);
	}
}