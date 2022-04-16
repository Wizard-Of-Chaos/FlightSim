#include "DefaultAIBehaviors.h"
#include "SceneManager.h"
#include <iostream>

void defaultIdleBehavior(EntityId id, f32 dt)
{
	auto rbc = sceneManager->scene.get<BulletRigidBodyComponent>(id);
	auto ship = sceneManager->scene.get<ShipComponent>(id);

	btVector3 force = btVector3(0, 0, 0);
	btVector3 torque = btVector3(0, 0, 0);

	force += getForceToStopLinearVelocity(&rbc->rigidBody, ship);
	torque += getTorqueToStopAngularVelocity(&rbc->rigidBody, ship);

	//slows and stops the rigid body
	rbc->rigidBody.applyTorqueImpulse(torque * dt);
	rbc->rigidBody.applyCentralImpulse(force * dt);

	for (unsigned int i = 0; i < ship->hardpointCount; ++i) {
		EntityId wep = ship->weapons[i];
		auto wepInfo = sceneManager->scene.get<WeaponInfoComponent>(wep);
		wepInfo->isFiring = false;
	}
}

void defaultFleeBehavior(EntityId id, EntityId fleeTarget, f32 dt)
{
	if (fleeTarget == INVALID_ENTITY) return;

	auto irr = sceneManager->scene.get<IrrlichtComponent>(id);
	auto rbc = sceneManager->scene.get<BulletRigidBodyComponent>(id);
	auto ship = sceneManager->scene.get<ShipComponent>(id);
	auto ai = sceneManager->scene.get<AIComponent>(id);

	auto fleeIrr = sceneManager->scene.get<IrrlichtComponent>(fleeTarget);
	if (!fleeIrr) return;
	vector3df distance = fleeIrr->node->getPosition() - irr->node->getPosition();
	distance.normalize();
	vector3df targetVector = -distance; //runs away

	btVector3 force = btVector3(0, 0, 0);
	btVector3 torque = btVector3(0, 0, 0);

	force += getForceForward(&rbc->rigidBody, ship);

	smoothTurnToDirection(&rbc->rigidBody, ship, irrVecToBt(targetVector));
	rbc->rigidBody.applyCentralImpulse(force * dt);

	for (unsigned int i = 0; i < ship->hardpointCount; ++i) {
		EntityId wep = ship->weapons[i];
		auto wepInfo = sceneManager->scene.get<WeaponInfoComponent>(wep);
		wepInfo->isFiring = false;
	}
}

//TLDR is try and get behind the ship and match its velocity.
void defaultPursuitBehavior(EntityId id, EntityId pursuitTarget, f32 dt)
{
	auto sensors = sceneManager->scene.get<SensorComponent>(id);

	if (pursuitTarget == INVALID_ENTITY) {
		sensors->targetContact = INVALID_ENTITY;
		return; 
	}

	auto rbc = sceneManager->scene.get<BulletRigidBodyComponent>(id);
	auto ship = sceneManager->scene.get<ShipComponent>(id);
	auto ai = sceneManager->scene.get<AIComponent>(id);

	sensors->targetContact = pursuitTarget;

	auto targetRBC = sceneManager->scene.get<BulletRigidBodyComponent>(pursuitTarget);
	if (!targetRBC) return;

	btVector3 targetPos = targetRBC->rigidBody.getCenterOfMassPosition();
	btVector3 pos = rbc->rigidBody.getCenterOfMassPosition();

	btVector3 tailPos = targetPos + (getRigidBodyBackward(&rbc->rigidBody) * 20.f);//targetPos();
	btVector3 dist = tailPos - pos;

	btVector3 facing = targetPos - pos;
	facing = facing.normalize();

	if (avoidObstacles(id, dt)) return;

	//If it's not behind the ship, get behind it
	if (dist.length() > 30.f) {
		goToPoint(&rbc->rigidBody, ship, tailPos, dt);
	}
	else {
		//if it is behind it, start turning towards it
		ship->moves[SHIP_STOP_VELOCITY] = true;
		smoothTurnToDirection(&rbc->rigidBody, ship, facing);
		auto pursuitBody = sceneManager->scene.get<BulletRigidBodyComponent>(sensors->closestContact);
	}

	btVector3 forward = getRigidBodyForward(&rbc->rigidBody);
	btScalar angle = forward.angle(facing);
	//if it's facing the ship, start shooting
	if ((angle * RADTODEG) < 30.f) {
		for (unsigned int i = 0; i < ship->hardpointCount; ++i) {
			EntityId wep = ship->weapons[i];
			auto wepInfo = sceneManager->scene.get<WeaponInfoComponent>(wep);
			auto irrComp = sceneManager->scene.get<IrrlichtComponent>(wep);
			wepInfo->isFiring = true;
			wepInfo->spawnPosition = irrComp->node->getAbsolutePosition() + (getNodeForward(irrComp->node) * 1.f);
			wepInfo->firingDirection = btVecToIrr(facing); 
		}
	}
	else {
		for (unsigned int i = 0; i < ship->hardpointCount; ++i) {
			EntityId wep = ship->weapons[i];
			if (!sceneManager->scene.entityInUse(wep)) continue;
			auto wepInfo = sceneManager->scene.get<WeaponInfoComponent>(wep);
			wepInfo->isFiring = false;
		}
	}
}