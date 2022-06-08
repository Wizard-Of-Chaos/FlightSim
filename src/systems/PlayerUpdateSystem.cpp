#include "PlayerUpdateSystem.h"
#include "GameController.h"
#include "GameStateController.h"
#include "BulletRigidBodyComponent.h"
#include "FactionComponent.h"

#include <iostream>
#include <cmath>
#include <algorithm>

void playerUpdateSystem(flecs::iter it, IrrlichtComponent* irrc, PlayerComponent* plyc, BulletRigidBodyComponent* rbcs, SensorComponent* snsc)
{
	for (auto i : it) {
		auto irr = &irrc[i];
		auto player = &plyc[i];
		auto rbc = &rbcs[i];
		auto sensors = &snsc[i];
		auto entity = it.entity(i);

		for (ContactInfo info : sensors->contacts) { //checks contacts and updates hud if they dont exist
			flecs::entity id = std::get<0>(info);
			if (player->trackedContacts[id] == nullptr && id.is_alive()) {
				HUDContact* ct = new HUDContact(player->rootHUD, id, entity);
				player->HUD.push_back(ct);
				player->trackedContacts[id] = ct;
			}
		}
		for (auto [id, hud] : player->trackedContacts) { //checks hud contacts and removes ones not in use
			flecs::entity ent(game_world->get_world(), id);
			if (!ent.is_alive()) {
				if (hud) player->removeContact(hud);
				continue;
			}
		}
		//camera work
		cameraUpdate(player, irr->node, &rbc->rigidBody);
		//HUD work and updates
		hudUpdate(player, entity);

		//updates the listener position for sound
		soundEngine->setListenerPosition(player->camera->getAbsolutePosition(), getNodeForward(player->camera), vec3df(0, 0, 0), getNodeUp(player->camera));
	}
}
/*
void playerUpdateSystem(Scene& scene, f32 frameDelta)
{
	for (auto entityId : SceneView<IrrlichtComponent, PlayerComponent, BulletRigidBodyComponent, InputComponent, SensorComponent>(scene)) {
		IrrlichtComponent* irrcomp = scene.get<IrrlichtComponent>(entityId);
		PlayerComponent* player = scene.get<PlayerComponent>(entityId);
		BulletRigidBodyComponent* rbc = scene.get<BulletRigidBodyComponent>(entityId);

		SensorComponent* sensors = scene.get<SensorComponent>(entityId);

		for (ContactInfo info : sensors->contacts) {
			EntityId id = std::get<0>(info);
			if (player->trackedContacts[id] == nullptr && scene.entityInUse(id)) {
				HUDContact* ct = new HUDContact(player->rootHUD, id, entityId);
				player->HUD.push_back(ct);
				player->trackedContacts[id] = ct;
			}
		}
		for (auto [id, hud] : player->trackedContacts) {
			if (!scene.entityInUse(id)) {
				if (hud) player->removeContact(hud);
				continue;
			}
		}

		//camera work
		cameraUpdate(player, irrcomp->node, &rbc->rigidBody);

		//HUD work
		hudUpdate(player, entityId);

		soundEngine->setListenerPosition(player->camera->getAbsolutePosition(), getNodeForward(player->camera), vec3df(0, 0, 0), getNodeUp(player->camera));
	}
}
*/

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

	f32 slerp = (abs(angle) + abs(rollAngle));
	quaternion desiredRot;
	vector3df targetRotVec(0, 0, 0);
	desiredRot.slerp(targetRot, nodeRot, player->slerpFactor*(slerp*5.f)); //remove the magic number here later
	desiredRot.toEuler(targetRotVec);
	targetRotVec *= RADTODEG;

	targetnode->setRotation(targetRotVec);

	vector3df targetUp = getNodeUp(targetnode);
	camera->setUpVector(targetUp);
	vector3df target = playerShip->getPosition() + (getNodeUp(playerShip) * 5.f) + btVecToIrr(body->getLinearVelocity()) * player->velocityFactor;
	camera->setTarget(target);
}

void hudUpdate(PlayerComponent* player, flecs::entity playerId)
{
	player->rootHUD->setRelativePosition(rect<s32>(position2di(0, 0), driver->getScreenSize()));
	for (HUDElement* elem : player->HUD) {
		elem->updateElement(playerId);
	}
}