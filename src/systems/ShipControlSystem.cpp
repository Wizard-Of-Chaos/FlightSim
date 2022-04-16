#include "ShipControlSystem.h"
#include "SceneManager.h"
#include "GameController.h"
#include <iostream>
#include <algorithm>

void checkSpaceFriction(ShipComponent* ship, vector3df& throttle, vector3df& rotate)
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
	if (!thrust) {
		ship->moves[SHIP_STOP_VELOCITY] = true;
		throttle = vector3df(0, 0, 0);
	}
	if (!rot) {
		ship->moves[SHIP_STOP_ROTATION] = true;
		rotate = vector3df(0, 0, 0);
	}
}

void setVelocity(ShipComponent* ship, f32& local, f32& desired, SHIP_MOVEMENT pos, SHIP_MOVEMENT neg)
{
	if (std::abs(local) < std::abs(desired)) {
		if (desired > 0) {
			ship->moves[pos] = true;
		} if (desired < 0) {
			ship->moves[neg] = true;
		}
	}
	else if (std::abs(local) > std::abs(desired)) {
		if (desired > 0) {
			ship->moves[neg] = true;
		} if (desired < 0) {
			ship->moves[pos] = true;
		}
	}
}

void throttleToShip(ShipComponent* ship, btRigidBody* body, vector3df& thrust, vector3df& rot)
{
	f32 desiredZ = thrust.Z * ship->linearMaxVelocity;
	f32 desiredY = thrust.Y * ship->linearMaxVelocity;
	f32 desiredX = thrust.X * ship->linearMaxVelocity;

	btVector3 ang = body->getAngularVelocity();
	btVector3 lin = body->getLinearVelocity();

	btScalar localZ = lin.dot(getRigidBodyForward(body));
	btScalar localX = lin.dot(getRigidBodyRight(body));
	btScalar localY = lin.dot(getRigidBodyUp(body));


	setVelocity(ship, localZ, desiredZ, SHIP_THRUST_FORWARD, SHIP_THRUST_BACKWARD);
	setVelocity(ship, localY, desiredY, SHIP_STRAFE_UP, SHIP_STRAFE_DOWN);
	setVelocity(ship, localX, desiredX, SHIP_STRAFE_RIGHT, SHIP_STRAFE_LEFT);

	f32 desiredPitch = rot.X * ship->angularMaxVelocity;
	f32 desiredYaw = rot.Y * ship->angularMaxVelocity;
	f32 desiredRoll = rot.Z * ship->angularMaxVelocity;

	f32 localPitch = ang.dot(getRigidBodyRight(body));
	f32 localYaw = ang.dot(getRigidBodyUp(body));
	f32 localRoll = ang.dot(getRigidBodyForward(body));

	std::cout << localPitch << ", " << desiredPitch << std::endl;
	setVelocity(ship, localPitch, desiredPitch, SHIP_PITCH_DOWN, SHIP_PITCH_UP);
	setVelocity(ship, localYaw, desiredYaw, SHIP_YAW_RIGHT, SHIP_YAW_LEFT);
	setVelocity(ship, localRoll, desiredRoll, SHIP_ROLL_RIGHT, SHIP_ROLL_LEFT);
}

void fireWeapon(PlayerComponent* player, IrrlichtComponent* playerIrr, InputComponent* input, EntityId wep)
{
	auto wepInfo = sceneManager->scene.get<WeaponInfoComponent>(wep);
	auto irrComp = sceneManager->scene.get<IrrlichtComponent>(wep);
	wepInfo->isFiring = true;
	wepInfo->spawnPosition = irrComp->node->getAbsolutePosition() + (getNodeForward(irrComp->node) * 1.f);
	vector3df target = input->cameraRay.getMiddle();
	ISceneNode* coll = smgr->getSceneCollisionManager()->getSceneNodeFromRayBB(input->cameraRay, ID_IsSelectable);

	bool mouseOverHUD = false;
	//auto-aim for aiming at a contact
	for (HUDElement* h : player->HUD) {
		if (h->type != HUD_ELEM_TYPE::CONTACT) continue;

		HUDContact* cont = (HUDContact*)h;
		if (cont->contactView->getAbsolutePosition().isPointInside(input->mousePixPosition)) {
			for (auto [id, c] : player->trackedContacts) {
				if (c != cont) continue;
				auto targetIrr = sceneManager->scene.get<IrrlichtComponent>(id);
				target = targetIrr->node->getPosition();
				mouseOverHUD = true;
				break;
			}
		}
		if (mouseOverHUD) break;
	}
	//auto-aim for aiming at whatever your cursor is hovering over
	if (!mouseOverHUD && coll && coll != playerIrr->node) {
		target = coll->getPosition();
	}
	vector3df dir = target - wepInfo->spawnPosition;
	dir.normalize();
	wepInfo->firingDirection = dir;
}

void shipControlSystem(f32 dt)
{ //This whole thing needs to be abstracted out to player-defined keybinds
	Scene& scene = sceneManager->scene;
	for(auto entityId : SceneView<InputComponent, ShipComponent, PlayerComponent, BulletRigidBodyComponent>(scene)) {
		InputComponent* input = scene.get<InputComponent>(entityId);
		ShipComponent* ship = scene.get<ShipComponent>(entityId);
		PlayerComponent* player = scene.get<PlayerComponent>(entityId);
		BulletRigidBodyComponent* rbc = scene.get<BulletRigidBodyComponent>(entityId);
		IrrlichtComponent* irr = scene.get<IrrlichtComponent>(entityId);

		//strafing
		ship->safetyOverride = input->safetyOverride;

		bool fa = gameController->gameConfig.flightAssist;

		if(input->isKeyDown(KEY_KEY_W)) {
			player->thrust.Z += dt;
			if (!fa) ship->moves[SHIP_THRUST_FORWARD] = true;
		}
		if(input->isKeyDown(KEY_KEY_S)) {
			player->thrust.Z -= dt;
			if (!fa) ship->moves[SHIP_THRUST_BACKWARD] = true;
		}
		player->thrust.Z = std::clamp(player->thrust.Z, -1.f, 1.f);

		if(input->isKeyDown(KEY_KEY_A)) {
			player->thrust.X -= dt;
			if (!fa) ship->moves[SHIP_STRAFE_LEFT] = true;
		}
		if(input->isKeyDown(KEY_KEY_D)) {
			player->thrust.X += dt;
			if (!fa) ship->moves[SHIP_STRAFE_RIGHT] = true;
		}
		player->thrust.X = std::clamp(player->thrust.X, -1.f, 1.f);
		if(input->isKeyDown(KEY_SPACE)) {
			player->thrust.Y += dt;
			if (!fa) ship->moves[SHIP_STRAFE_UP] = true;
		}
		if(input->isKeyDown(KEY_LCONTROL)) {
			player->thrust.Y -= dt;
			if (!fa) ship->moves[SHIP_STRAFE_DOWN] = true;
		}
		player->thrust.Y = std::clamp(player->thrust.Y, -1.f, 1.f);
		if (input->isKeyDown(KEY_TAB)) {
			ship->moves[SHIP_AFTERBURNER] = true;
		}

		//rotations
		if (input->isKeyDown(KEY_KEY_C)) {
			player->rotation.Z -= dt;
			if (!fa) ship->moves[SHIP_ROLL_RIGHT] = true;
		}
		if(input->isKeyDown(KEY_KEY_Z)) {
			player->rotation.Z += dt;
			if (!fa) ship->moves[SHIP_ROLL_LEFT] = true;
		}
		player->rotation.Z = std::clamp(player->rotation.Z, -1.f, 1.f);
		if(input->isKeyDown(KEY_KEY_R)) {
			player->rotation.X -= dt;
			if (!fa) ship->moves[SHIP_PITCH_UP] = true;
		}
		if(input->isKeyDown(KEY_KEY_F)) {
			player->rotation.X += dt;
			if (!fa) ship->moves[SHIP_PITCH_DOWN] = true;
		}
		player->rotation.X = std::clamp(player->rotation.X, -1.f, 1.f);
		if (input->isKeyDown(KEY_KEY_Q)) {
			player->rotation.Y -= dt;
			if (!fa) ship->moves[SHIP_YAW_LEFT] = true;
		}
		if(input->isKeyDown(KEY_KEY_E)) {
			player->rotation.Y += dt;
			if (!fa) ship->moves[SHIP_YAW_RIGHT] = true;
		}
		player->rotation.Y = std::clamp(player->rotation.Y, -1.f, 1.f);

		//STOOOOOOOOOOOOOOOOOOOP
		if (input->isKeyDown(KEY_KEY_X)) {
			ship->moves[SHIP_STOP_ROTATION] = true;
			ship->moves[SHIP_STOP_VELOCITY] = true;
			player->thrust = vector3df(0, 0, 0);
			player->rotation = vector3df(0, 0, 0);
		}

		if (fa) {
			throttleToShip(ship, &rbc->rigidBody, player->thrust, player->rotation);
		}

		if (gameController->gameConfig.spaceFriction) {
			checkSpaceFriction(ship, player->thrust, player->rotation);
		}

		input->cameraRay = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(input->mousePixPosition, player->camera);
		ISceneNode* coll = smgr->getSceneCollisionManager()->getSceneNodeFromRayBB(input->cameraRay, ID_IsSelectable);

		if (input->mouseControlEnabled) {
			vector3df viewpoint = input->cameraRay.getMiddle();
			viewpoint = viewpoint - btVecToIrr(rbc->rigidBody.getCenterOfMassPosition());
			viewpoint.normalize();

			btScalar angle = getRigidBodyForward(&rbc->rigidBody).angle(irrVecToBt(viewpoint));
			btVector3 ang = rbc->rigidBody.getAngularVelocity();
			if (angle * RADTODEG >= .8f) {
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
				fireWeapon(player, irr, input, wep);
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

		if (input->rightMouseDown) {
			fireWeapon(player, irr, input, ship->physWeapon);
		}
		else {
			auto wepInfo = scene.get<WeaponInfoComponent>(ship->physWeapon);
			wepInfo->isFiring = false;
		}
	}
}