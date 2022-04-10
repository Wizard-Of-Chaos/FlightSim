#include "DefaultAIBehaviors.h"
#include "SceneManager.h"
#include <iostream>

void defaultIdleBehavior(SceneManager* manager, EntityId id, f32 dt)
{
	auto rbc = manager->scene.get<BulletRigidBodyComponent>(id);
	auto ship = manager->scene.get<ShipComponent>(id);

	btVector3 force = btVector3(0, 0, 0);
	btVector3 torque = btVector3(0, 0, 0);

	force += getForceToStopLinearVelocity(&rbc->rigidBody, ship);
	torque += getTorqueToStopAngularVelocity(&rbc->rigidBody, ship);

	//slows and stops the rigid body
	rbc->rigidBody.applyTorqueImpulse(torque * dt);
	rbc->rigidBody.applyCentralImpulse(force * dt);

	for (unsigned int i = 0; i < ship->hardpointCount; ++i) {
		EntityId wep = ship->weapons[i];
		auto wepInfo = manager->scene.get<WeaponInfoComponent>(wep);
		wepInfo->isFiring = false;
	}
}

void defaultFleeBehavior(SceneManager* manager, EntityId id, EntityId fleeTarget, f32 dt)
{
	if (fleeTarget == INVALID_ENTITY) return;

	auto irr = manager->scene.get<IrrlichtComponent>(id);
	auto rbc = manager->scene.get<BulletRigidBodyComponent>(id);
	auto ship = manager->scene.get<ShipComponent>(id);
	auto ai = manager->scene.get<AIComponent>(id);

	auto fleeIrr = manager->scene.get<IrrlichtComponent>(fleeTarget);
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
		auto wepInfo = manager->scene.get<WeaponInfoComponent>(wep);
		wepInfo->isFiring = false;
	}
}

//TLDR is try and get behind the ship and match its velocity.
void defaultPursuitBehavior(SceneManager* manager, EntityId id, EntityId pursuitTarget, f32 dt)
{
	auto sensors = manager->scene.get<SensorComponent>(id);

	if (pursuitTarget == INVALID_ENTITY) {
		sensors->targetContact = INVALID_ENTITY;
		return; 
	}

	auto rbc = manager->scene.get<BulletRigidBodyComponent>(id);
	auto ship = manager->scene.get<ShipComponent>(id);
	auto ai = manager->scene.get<AIComponent>(id);

	sensors->targetContact = pursuitTarget;

	auto targetRBC = manager->scene.get<BulletRigidBodyComponent>(pursuitTarget);
	if (!targetRBC) return;

	btVector3 targetPos = targetRBC->rigidBody.getCenterOfMassPosition();
	btVector3 pos = rbc->rigidBody.getCenterOfMassPosition();

	btVector3 tailPos = targetPos + (getRigidBodyBackward(&rbc->rigidBody) * 20.f);//targetPos();
	btVector3 dist = tailPos - pos;

	btVector3 facing = targetPos - pos;
	facing = facing.normalize();

	if (avoidObstacles(manager, id, dt)) return;

	//If it's not behind the ship, get behind it
	if (dist.length() > 30.f) {
		goToPoint(&rbc->rigidBody, ship, tailPos, dt);
	}
	else {
		//if it is behind it, start turning towards it
		ship->moves[SHIP_STOP_VELOCITY] = true;
		smoothTurnToDirection(&rbc->rigidBody, ship, facing);
		auto pursuitBody = manager->scene.get<BulletRigidBodyComponent>(sensors->closestContact);
	}

	btVector3 forward = getRigidBodyForward(&rbc->rigidBody);
	btScalar angle = forward.angle(facing);
	//if it's facing the ship, start shooting
	if ((angle * RADTODEG) < 30.f) {
		for (unsigned int i = 0; i < ship->hardpointCount; ++i) {
			EntityId wep = ship->weapons[i];
			auto wepInfo = manager->scene.get<WeaponInfoComponent>(wep);
			auto irrComp = manager->scene.get<IrrlichtComponent>(wep);
			wepInfo->isFiring = true;
			wepInfo->spawnPosition = irrComp->node->getAbsolutePosition() + (getNodeForward(irrComp->node) * 15.f);
			wepInfo->firingDirection = btVecToIrr(facing); //getNodeForward(irrComp->node);
		}
	}
	else {
		for (unsigned int i = 0; i < ship->hardpointCount; ++i) {
			EntityId wep = ship->weapons[i];
			if (!manager->scene.entityInUse(wep)) continue;
			auto wepInfo = manager->scene.get<WeaponInfoComponent>(wep);
			wepInfo->isFiring = false;
		}
	}
}