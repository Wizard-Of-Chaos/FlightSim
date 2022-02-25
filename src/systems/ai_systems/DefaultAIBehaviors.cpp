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

	smoothTurnToDirection(&rbc->rigidBody, ship, irrVecToBt(targetVector), dt);
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
	if (pursuitTarget == INVALID_ENTITY) return;

	auto irr = manager->scene.get<IrrlichtComponent>(id);
	auto rbc = manager->scene.get<BulletRigidBodyComponent>(id);
	auto ship = manager->scene.get<ShipComponent>(id);
	auto ai = manager->scene.get<AIComponent>(id);
	auto sensors = manager->scene.get<SensorComponent>(id);

	auto pursuitIrr = manager->scene.get<IrrlichtComponent>(pursuitTarget);
	if (!pursuitIrr) return;
	vector3df tailPos = getNodeBackward(pursuitIrr->node) * 30.f; //change to backward
	vector3df dist = tailPos - irr->node->getPosition();

	vector3df facing = pursuitIrr->node->getPosition() - irr->node->getPosition();
	facing.normalize();

	//If it's not behind the ship, get behind it
	if (dist.getLength() > 50.f) {
		goToPoint(&rbc->rigidBody, ship, irrVecToBt(tailPos), dt);
	}
	else {
		//if it is behind it, start turning towards it
		smoothTurnToDirection(&rbc->rigidBody, ship, irrVecToBt(facing), dt);
		auto pursuitBody = manager->scene.get<BulletRigidBodyComponent>(sensors->closestContact);
	}

	btVector3 forward = getRigidBodyForward(&rbc->rigidBody);
	btScalar angle = forward.angle(irrVecToBt(facing));
	//if it's facing the ship, start shooting
	if ((angle * RADTODEG) < 30.f) {
		for (unsigned int i = 0; i < ship->hardpointCount; ++i) {
			EntityId wep = ship->weapons[i];
			auto wepInfo = manager->scene.get<WeaponInfoComponent>(wep);
			wepInfo->isFiring = true;
		}
	}
	else {
		for (unsigned int i = 0; i < ship->hardpointCount; ++i) {
			EntityId wep = ship->weapons[i];
			auto wepInfo = manager->scene.get<WeaponInfoComponent>(wep);
			wepInfo->isFiring = false;
		}
	}
}