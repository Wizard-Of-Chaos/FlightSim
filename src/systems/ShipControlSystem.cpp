#include "ShipControlSystem.h"
#include "GameController.h"
#include <iostream>
#include <algorithm>

void checkSpaceFriction(ThrustComponent* thrustc, vector3df& throttle, vector3df& rotate)
{
	bool thrust = false;
	bool rot = false;
	for (u32 i = 0; i < 7; ++i) {
		if (thrustc->moves[i]) {
			thrust = true;
			break;
		}
	}
	for (u32 i = 7; i < 13; ++i) {
		if (thrustc->moves[i]) {
			rot = true;
			break;
		}
	}
	if (!thrust) {
		thrustc->moves[STOP_VELOCITY] = true;
		throttle = vector3df(0, 0, 0);
	}
	if (!rot) {
		thrustc->moves[STOP_ROTATION] = true;
		rotate = vector3df(0, 0, 0);
	}
}

void setVelocity(ThrustComponent* thrust, f32& local, f32& desired, MOVEMENT pos, MOVEMENT neg)
{
	if (std::abs(local) < std::abs(desired)) {
		if (desired > 0) {
			thrust->moves[pos] = true;
		} if (desired < 0) {
			thrust->moves[neg] = true;
		}
	}
	else if (std::abs(local) > std::abs(desired)) {
		if (desired > 0) {
			thrust->moves[neg] = true;
		} if (desired < 0) {
			thrust->moves[pos] = true;
		}
	}
}

void throttleToShip(ThrustComponent* thrustc, btRigidBody* body, vector3df& thrust, vector3df& rot)
{
	f32 desiredZ = thrust.Z * thrustc->linearMaxVelocity;
	f32 desiredY = thrust.Y * thrustc->linearMaxVelocity;
	f32 desiredX = thrust.X * thrustc->linearMaxVelocity;

	btVector3 ang = body->getAngularVelocity();
	btVector3 lin = body->getLinearVelocity();

	btScalar localZ = lin.dot(getRigidBodyForward(body));
	btScalar localX = lin.dot(getRigidBodyRight(body));
	btScalar localY = lin.dot(getRigidBodyUp(body));


	setVelocity(thrustc, localZ, desiredZ, THRUST_FORWARD, THRUST_BACKWARD);
	setVelocity(thrustc, localY, desiredY, STRAFE_UP, STRAFE_DOWN);
	setVelocity(thrustc, localX, desiredX, STRAFE_RIGHT, STRAFE_LEFT);

	f32 desiredPitch = rot.X * thrustc->angularMaxVelocity;
	f32 desiredYaw = rot.Y * thrustc->angularMaxVelocity;
	f32 desiredRoll = rot.Z * thrustc->angularMaxVelocity;

	f32 localPitch = ang.dot(getRigidBodyRight(body));
	f32 localYaw = ang.dot(getRigidBodyUp(body));
	f32 localRoll = ang.dot(getRigidBodyForward(body));

	std::cout << localPitch << ", " << desiredPitch << std::endl;
	setVelocity(thrustc, localPitch, desiredPitch, PITCH_DOWN, PITCH_UP);
	setVelocity(thrustc, localYaw, desiredYaw, YAW_RIGHT, YAW_LEFT);
	setVelocity(thrustc, localRoll, desiredRoll, ROLL_RIGHT, ROLL_LEFT);
}

void firePlayerWeapon(flecs::entity playerId, InputComponent* input, flecs::entity wep)
{
	if (!wep.is_alive() || !playerId.is_alive()) return;

	auto player = playerId.get<PlayerComponent>();
	auto sensors = playerId.get<SensorComponent>();
	auto playerIrr = playerId.get<IrrlichtComponent>();
	auto irrComp = wep.get<IrrlichtComponent>();

	if (!wep.has<WeaponInfoComponent>() || !player || !sensors || !playerIrr || !irrComp) return;
	auto wepInfo = wep.get_mut<WeaponInfoComponent>();

	wepInfo->isFiring = true;
	wepInfo->spawnPosition = irrComp->node->getAbsolutePosition() + (getNodeForward(irrComp->node) * 10.f);
	vector3df target = input->cameraRay.getMiddle();
	ISceneNode* coll = smgr->getSceneCollisionManager()->getSceneNodeFromRayBB(input->cameraRay, ID_IsSelectable);

	bool mouseOverHUD = false;
	//auto aim for aiming at the current active selection
	//auto-aim for aiming at a contact
	for (HUDElement* h : player->HUD) {
		if (h->type == HUD_ELEM_TYPE::ACTIVE_SELECTION) {
			if (!sensors->targetContact.is_alive()) continue;

			HUDActiveSelection* act = (HUDActiveSelection*)h;
			if (act->crosshair->getAbsolutePosition().isPointInside(input->mousePixPosition)) {
				auto rbc = playerId.get<BulletRigidBodyComponent>();
				btVector3 forwardTarget;

				if (sensors->targetContact.has<BulletRigidBodyComponent>()) {
					auto targetRBC = sensors->targetContact.get<BulletRigidBodyComponent>();
					forwardTarget = targetRBC->rigidBody->getCenterOfMassPosition() + (targetRBC->rigidBody->getLinearVelocity() * .35f);
				}
				else if (sensors->targetContact.has<IrrlichtComponent>()) {
					auto targetIrr = sensors->targetContact.get<IrrlichtComponent>();
					forwardTarget = irrVecToBt(targetIrr->node->getAbsolutePosition());
				}
				forwardTarget += (rbc->rigidBody->getLinearVelocity() * -.35f);
				target = btVecToIrr(forwardTarget);
				continue;
			}
		} else if (h->type != HUD_ELEM_TYPE::CONTACT) continue;

		HUDContact* cont = (HUDContact*)h;
		if (cont->contactView->getAbsolutePosition().isPointInside(input->mousePixPosition)) {
			for (auto [id, c] : player->trackedContacts) {
				if (c != cont) continue;
				flecs::entity ent(game_world->get_world(), id);
				auto targetIrr = ent.get<IrrlichtComponent>();
				if (!targetIrr) break;
				target = targetIrr->node->getPosition();
				mouseOverHUD = true;
				break;
			}
		}
		if (mouseOverHUD) break;
	}
	/*
	//auto-aim for aiming at whatever your cursor is hovering over
	if (!mouseOverHUD && coll && coll != playerIrr->node) {
		target = coll->getPosition();
	}
	*/
	vector3df dir = target - wepInfo->spawnPosition;
	dir.normalize();
	wepInfo->firingDirection = dir;
}

void shipControlSystem(flecs::iter it,
	InputComponent* inc, ShipComponent* shpc, ThrustComponent* thrc, PlayerComponent* plyc, BulletRigidBodyComponent* rbcs, IrrlichtComponent* irrc, SensorComponent* snsc)
{ //This whole thing needs to be abstracted out to player-defined keybinds
	for(auto i : it) {
		f32 dt = it.delta_time();
		InputComponent* input = &inc[i];
		ShipComponent* ship = &shpc[i];
		PlayerComponent* player = &plyc[i];
		BulletRigidBodyComponent* rbc = &rbcs[i];
		IrrlichtComponent* irr = &irrc[i];
		SensorComponent* sensors = &snsc[i];
		ThrustComponent* thrust = &thrc[i];

		flecs::entity entityId = it.entity(i);
		//strafing
		ship->safetyOverride = input->safetyOverride;

		bool fa = gameController->gameConfig.flightAssist;

		if(input->isKeyDown(KEY_KEY_W)) {
			player->thrust.Z += dt;
			if (!fa) thrust->moves[THRUST_FORWARD] = true;
		}
		if(input->isKeyDown(KEY_KEY_S)) {
			player->thrust.Z -= dt;
			if (!fa) thrust->moves[THRUST_BACKWARD] = true;
		}
		player->thrust.Z = std::clamp(player->thrust.Z, -1.f, 1.f);

		if(input->isKeyDown(KEY_KEY_A)) {
			player->thrust.X -= dt;
			if (!fa) thrust->moves[STRAFE_LEFT] = true;
		}
		if(input->isKeyDown(KEY_KEY_D)) {
			player->thrust.X += dt;
			if (!fa) thrust->moves[STRAFE_RIGHT] = true;
		}
		player->thrust.X = std::clamp(player->thrust.X, -1.f, 1.f);
		if(input->isKeyDown(KEY_SPACE)) {
			player->thrust.Y += dt;
			if (!fa) thrust->moves[STRAFE_UP] = true;
		}
		if(input->isKeyDown(KEY_LCONTROL)) {
			player->thrust.Y -= dt;
			if (!fa) thrust->moves[STRAFE_DOWN] = true;
		}
		player->thrust.Y = std::clamp(player->thrust.Y, -1.f, 1.f);
		if (input->isKeyDown(KEY_TAB)) {
			thrust->moves[THRUST_BOOST] = true;
		}

		//rotations
		if (input->isKeyDown(KEY_KEY_C)) {
			player->rotation.Z -= dt;
			if (!fa) thrust->moves[ROLL_RIGHT] = true;
		}
		if(input->isKeyDown(KEY_KEY_Z)) {
			player->rotation.Z += dt;
			if (!fa) thrust->moves[ROLL_LEFT] = true;
		}
		player->rotation.Z = std::clamp(player->rotation.Z, -1.f, 1.f);
		if(input->isKeyDown(KEY_KEY_R)) {
			player->rotation.X -= dt;
			if (!fa) thrust->moves[PITCH_UP] = true;
		}
		if(input->isKeyDown(KEY_KEY_F)) {
			player->rotation.X += dt;
			if (!fa) thrust->moves[PITCH_DOWN] = true;
		}
		player->rotation.X = std::clamp(player->rotation.X, -1.f, 1.f);
		if (input->isKeyDown(KEY_KEY_Q)) {
			player->rotation.Y -= dt;
			if (!fa) thrust->moves[YAW_LEFT] = true;
		}
		if(input->isKeyDown(KEY_KEY_E)) {
			player->rotation.Y += dt;
			if (!fa) thrust->moves[YAW_RIGHT] = true;
		}
		player->rotation.Y = std::clamp(player->rotation.Y, -1.f, 1.f);

		//STOOOOOOOOOOOOOOOOOOOP
		if (input->isKeyDown(KEY_KEY_X)) {
			thrust->moves[STOP_ROTATION] = true;
			thrust->moves[STOP_VELOCITY] = true;
			player->thrust = vector3df(0, 0, 0);
			player->rotation = vector3df(0, 0, 0);
		}

		if (fa) {
			throttleToShip(thrust, rbc->rigidBody, player->thrust, player->rotation);
		}

		if (gameController->gameConfig.spaceFriction) {
			checkSpaceFriction(thrust, player->thrust, player->rotation);
		}

		input->cameraRay = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(input->mousePixPosition, player->camera);
		ISceneNode* coll = smgr->getSceneCollisionManager()->getSceneNodeFromRayBB(input->cameraRay, ID_IsSelectable);

		if (input->mouseControlEnabled) {
			vector3df viewpoint = input->cameraRay.getMiddle();
			viewpoint = viewpoint - btVecToIrr(rbc->rigidBody->getCenterOfMassPosition());
			viewpoint.normalize();

			btScalar angle = getRigidBodyForward(rbc->rigidBody).angle(irrVecToBt(viewpoint));
			btVector3 ang = rbc->rigidBody->getAngularVelocity();
			if (angle * RADTODEG >= .8f) {
				turnToDirection(rbc->rigidBody, thrust, irrVecToBt(viewpoint));
			}
			else {
				thrust->moves[STOP_ROTATION] = true;
			}

		}

		if (input->leftMouseDown) {
			for (unsigned int i = 0; i < ship->hardpointCount; ++i) {
				flecs::entity wep = ship->weapons[i];
				if (!wep.is_alive()) continue;
				firePlayerWeapon(entityId, input, wep);
			}
		}
		else {
			for (unsigned int i = 0; i < ship->hardpointCount; ++i) {
				flecs::entity wep = ship->weapons[i];
				if (!wep.is_alive()) continue;

				auto wepInfo = wep.get_mut<WeaponInfoComponent>();
				wepInfo->isFiring = false;
			}
		}

		if (input->rightMouseDown) {
			firePlayerWeapon(entityId, input, ship->physWeapon);
		}
		else {
			if (ship->physWeapon.is_alive()) {
				auto wepInfo = ship->physWeapon.get_mut<WeaponInfoComponent>();
				if(wepInfo) wepInfo->isFiring = false;
			}
		}
	}
}