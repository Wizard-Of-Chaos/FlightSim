#include "ShipControlSystem.h"
#include "SceneManager.h"
#include "GameController.h"
#include <iostream>

void checkSpaceFriction(ShipComponent* ship)
{
	bool thrust = false;
	bool rot = false;
	for (u32 i = 0; i < 7; ++i) {
		if (ship->moves[i]) {
			thrust = true;
			break;
		}
	}
	for (u32 i = 7; i < 13; ++i) {
		if (ship->moves[i]) {
			rot = true;
			break;
		}
	}
	if (!thrust) ship->moves[SHIP_STOP_VELOCITY] = true;
	if (!rot) ship->moves[SHIP_STOP_ROTATION] = true;
}

void shipControlSystem(SceneManager* manager, f32 dt)
{ //This whole thing needs to be abstracted out to player-defined keybinds
	Scene& scene = manager->scene;
	for(auto entityId : SceneView<InputComponent, ShipComponent, PlayerComponent, BulletRigidBodyComponent>(scene)) {
		InputComponent* input = scene.get<InputComponent>(entityId);
		ShipComponent* ship = scene.get<ShipComponent>(entityId);
		PlayerComponent* player = scene.get<PlayerComponent>(entityId);
		BulletRigidBodyComponent* rbc = scene.get<BulletRigidBodyComponent>(entityId);

		//strafing
		ship->safetyOverride = input->safetyOverride;

		if(input->isKeyDown(KEY_KEY_W)) {
			ship->moves[SHIP_THRUST_FORWARD] = true;
		}
		if(input->isKeyDown(KEY_KEY_S)) {
			ship->moves[SHIP_STRAFE_BACKWARD] = true;
		}
		if(input->isKeyDown(KEY_KEY_A)) {
			ship->moves[SHIP_STRAFE_LEFT] = true;
		}
		if(input->isKeyDown(KEY_KEY_D)) {
			ship->moves[SHIP_STRAFE_RIGHT] = true;
		}
		if(input->isKeyDown(KEY_SPACE)) {
			ship->moves[SHIP_STRAFE_UP] = true;
		}
		if(input->isKeyDown(KEY_LCONTROL)) {
			ship->moves[SHIP_STRAFE_DOWN] = true;
		}
		if (input->isKeyDown(KEY_TAB)) {
			ship->moves[SHIP_AFTERBURNER] = true;
		}

		//rotations
		if(input->isKeyDown(KEY_KEY_Z)) {
			ship->moves[SHIP_ROLL_LEFT] = true;
		}
		if(input->isKeyDown(KEY_KEY_C)) {
			ship->moves[SHIP_ROLL_RIGHT] = true;
		}
		if(input->isKeyDown(KEY_KEY_R)) {
			ship->moves[SHIP_PITCH_UP] = true;
		}
		if(input->isKeyDown(KEY_KEY_F)) {
			ship->moves[SHIP_PITCH_DOWN] = true;
		}
		if (input->isKeyDown(KEY_KEY_Q)) {
			ship->moves[SHIP_YAW_LEFT] = true;
		}
		if(input->isKeyDown(KEY_KEY_E)) {
			ship->moves[SHIP_YAW_RIGHT] = true;
		}

		//STOOOOOOOOOOOOOOOOOOOP
		if (input->isKeyDown(KEY_KEY_X)) {
			ship->moves[SHIP_STOP_ROTATION] = true;
			ship->moves[SHIP_STOP_VELOCITY] = true;
		}

		if (manager->controller->gameConfig.spaceFriction) {
			checkSpaceFriction(ship);
		}
		input->cameraRay = manager->controller->smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(input->mousePixPosition, player->camera);

		if (input->mouseControlEnabled) {
			vector3df viewpoint = input->cameraRay.getMiddle();
			viewpoint.normalize();

			btScalar angle = getRigidBodyForward(&rbc->rigidBody).angle(irrVecToBt(viewpoint));
			btVector3 ang = rbc->rigidBody.getAngularVelocity();
			if (angle * RADTODEG >= 3.f) {
				turnToDirection(&rbc->rigidBody, ship, irrVecToBt(viewpoint));
			}
			else {
				ship->moves[SHIP_STOP_ROTATION] = true;
			}
		}

		if (input->leftMouseDown) {
			for (unsigned int i = 0; i < ship->hardpointCount; ++i) {
				EntityId wep = ship->weapons[i];
				if (!scene.entityInUse(wep)) continue;

				auto wepInfo = scene.get<WeaponInfoComponent>(wep);
				auto irrComp = scene.get<IrrlichtComponent>(wep);
				wepInfo->isFiring = true;
				wepInfo->spawnPosition = irrComp->node->getAbsolutePosition() + (getNodeForward(irrComp->node) * 15.f);
				vector3df target = input->cameraRay.getMiddle();
				vector3df dir = target - wepInfo->spawnPosition;
				dir.normalize();
				wepInfo->firingDirection = dir;
			}
		}
		else {
			for (unsigned int i = 0; i < ship->hardpointCount; ++i) {
				EntityId wep = ship->weapons[i];
				if (!scene.entityInUse(wep)) continue;

				auto wepInfo = scene.get<WeaponInfoComponent>(wep);
				wepInfo->isFiring = false;
			}
		}

	}
}