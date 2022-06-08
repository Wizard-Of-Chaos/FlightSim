#include "DefaultAIBehaviors.h"
#include "WeaponInfoComponent.h"
#include "BulletRigidBodyComponent.h"
#include "SensorComponent.h"
#include "IrrlichtComponent.h"
#include "IrrlichtUtils.h"
#include "ShipMovementUtils.h"

#include <iostream>

void setAIWeapon(flecs::entity wep, bool firing)
{
	if (!wep.is_alive()) return;
	if (!wep.has<WeaponInfoComponent>()) return;
	auto wepInfo = wep.get_mut<WeaponInfoComponent>();
	wepInfo->isFiring = firing;

}

void defaultIdleBehavior(flecs::entity id, f32 dt)
{
	if (!id.is_alive()) return;
	if (!id.has<BulletRigidBodyComponent>() || !id.has<ShipComponent>()) return;

	auto rbc = id.get_mut<BulletRigidBodyComponent>();
	auto ship = id.get_mut<ShipComponent>();

	btVector3 force = btVector3(0, 0, 0);
	btVector3 torque = btVector3(0, 0, 0);

	force += getForceToStopLinearVelocity(&rbc->rigidBody, ship);
	torque += getTorqueToStopAngularVelocity(&rbc->rigidBody, ship);

	//slows and stops the rigid body
	rbc->rigidBody.applyTorqueImpulse(torque * dt);
	rbc->rigidBody.applyCentralImpulse(force * dt);

	for (unsigned int i = 0; i < ship->hardpointCount; ++i) {
		flecs::entity wep = ship->weapons[i];
		setAIWeapon(wep, false);
	}
}

void defaultFleeBehavior(flecs::entity id, flecs::entity fleeTarget, f32 dt)
{
	if (fleeTarget == INVALID_ENTITY || !id.is_alive()) return;
	if (!id.has<IrrlichtComponent>() || !id.has<BulletRigidBodyComponent>() || !id.has<ShipComponent>()) return;

	auto irr = id.get<IrrlichtComponent>();
	auto rbc = id.get_mut<BulletRigidBodyComponent>();
	auto ship = id.get_mut<ShipComponent>();

	if (!fleeTarget.has<IrrlichtComponent>()) return;
	auto fleeIrr = fleeTarget.get<IrrlichtComponent>();

	vector3df distance = fleeIrr->node->getPosition() - irr->node->getPosition();
	distance.normalize();
	vector3df targetVector = -distance; //runs away

	btVector3 force = btVector3(0, 0, 0);
	btVector3 torque = btVector3(0, 0, 0);

	force += getForceForward(&rbc->rigidBody, ship);

	smoothTurnToDirection(&rbc->rigidBody, ship, irrVecToBt(targetVector));
	rbc->rigidBody.applyCentralImpulse(force * dt);

	for (unsigned int i = 0; i < ship->hardpointCount; ++i) {
		setAIWeapon(ship->weapons[i], false);
	}
}

//TLDR is try and get behind the ship and match its velocity.
void defaultPursuitBehavior(flecs::entity id, flecs::entity pursuitTarget, f32 dt)
{
	if (!id.has<SensorComponent>() || !id.has<BulletRigidBodyComponent>() || !id.has<ShipComponent>()) return;
	auto sensors = id.get_mut<SensorComponent>();

	if (pursuitTarget == INVALID_ENTITY || !pursuitTarget.is_alive()) {
		sensors->targetContact = INVALID_ENTITY;
		return; 
	}
	auto rbc = id.get_mut<BulletRigidBodyComponent>();
	auto ship = id.get_mut<ShipComponent>();

	sensors->targetContact = pursuitTarget;

	if (!pursuitTarget.has<BulletRigidBodyComponent>()) return;
	auto targetRBC = pursuitTarget.get<BulletRigidBodyComponent>();

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
	}

	btVector3 forward = getRigidBodyForward(&rbc->rigidBody);
	btScalar angle = forward.angle(facing);
	//if it's facing the ship, start shooting
	if ((angle * RADTODEG) < 30.f) { //converted to degrees so my overworked meat brain can better comprehend it
		for (unsigned int i = 0; i < ship->hardpointCount; ++i) {
			flecs::entity wep = ship->weapons[i];
			if (!wep.is_alive()) continue;
			if (!wep.has<WeaponInfoComponent>() || !wep.has<IrrlichtComponent>()) continue;
			auto wepInfo = wep.get_mut<WeaponInfoComponent>();
			auto irrComp = wep.get_mut<IrrlichtComponent>();
			wepInfo->isFiring = true;
			wepInfo->spawnPosition = irrComp->node->getAbsolutePosition() + (getNodeForward(irrComp->node) * 1.f);
			wepInfo->firingDirection = btVecToIrr(facing); //todo: the ai doesn't lead its shots at all, fix it
		}
	}
	else {
		for (unsigned int i = 0; i < ship->hardpointCount; ++i) {
			setAIWeapon(ship->weapons[i], false);
		}
	}
}