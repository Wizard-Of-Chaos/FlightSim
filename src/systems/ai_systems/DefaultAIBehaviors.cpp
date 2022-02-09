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

	rbc->rigidBody.applyTorqueImpulse(torque * dt);
	rbc->rigidBody.applyCentralImpulse(force * dt);
}

void defaultFleeBehavior(SceneManager* manager, EntityId id, f32 dt)
{
	auto irr = manager->scene.get<IrrlichtComponent>(id);
	auto rbc = manager->scene.get<BulletRigidBodyComponent>(id);
	auto ship = manager->scene.get<ShipComponent>(id);
	auto ai = manager->scene.get<AIComponent>(id);

	auto fleeIrr = manager->scene.get<IrrlichtComponent>(ai->closestContact);
	vector3df distance = fleeIrr->node->getPosition() - irr->node->getPosition();
	distance.normalize();
	vector3df targetVector = -distance; //runs away

	btVector3 force = btVector3(0, 0, 0);
	btVector3 torque = btVector3(0, 0, 0);

	force += getForceForward(&rbc->rigidBody, ship);

	smoothTurnToDirection(&rbc->rigidBody, ship, irrlichtVectorToBullet(targetVector), dt);
	rbc->rigidBody.applyCentralImpulse(force * dt);
}

//TLDR is try and get behind the ship and match its velocity.
void defaultPursuitBehavior(SceneManager* manager, EntityId id, f32 dt)
{
	auto irr = manager->scene.get<IrrlichtComponent>(id);
	auto rbc = manager->scene.get<BulletRigidBodyComponent>(id);
	auto ship = manager->scene.get<ShipComponent>(id);
	auto ai = manager->scene.get<AIComponent>(id);

	auto pursuitIrr = manager->scene.get<IrrlichtComponent>(ai->closestContact);
	vector3df tailPos = getNodeBackward(pursuitIrr->node) * 30.f; //change to backward
	vector3df dist = tailPos - irr->node->getPosition();

	vector3df facing = pursuitIrr->node->getPosition() - irr->node->getPosition();
	facing.normalize();

	//If it's not behind the ship, get behind it
	if (dist.getLength() > 50.f) {
		goToPoint(&rbc->rigidBody, ship, irrlichtVectorToBullet(tailPos), dt);
	}
	else {
		//if it is behind it, start turning towards it
		smoothTurnToDirection(&rbc->rigidBody, ship, irrlichtVectorToBullet(facing), dt);
		auto pursuitBody = manager->scene.get<BulletRigidBodyComponent>(ai->closestContact);
		
		//start trying to match its velocity
		if (rbc->rigidBody.getLinearVelocity().length2() >= pursuitBody->rigidBody.getLinearVelocity().length2()) {
			rbc->rigidBody.applyCentralImpulse(getForceToStopLinearVelocity(&rbc->rigidBody, ship) * dt);
		}
		else {
			rbc->rigidBody.applyCentralImpulse(getForceForward(&rbc->rigidBody, ship) * dt);
		}
	}

	btVector3 forward = getRigidBodyForward(&rbc->rigidBody);
	btScalar angle = forward.angle(irrlichtVectorToBullet(facing));
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