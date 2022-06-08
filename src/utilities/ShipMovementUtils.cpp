#include "ShipMovementUtils.h"
#include "GameController.h"
#include "GameStateController.h"

#include <iostream>

void QuaternionToEuler(const btQuaternion& TQuat, btVector3& TEuler) {
	btScalar x, y, z;

	TQuat.getEulerZYX(z, y, x);
	TEuler.setValue(x, y, z);
	TEuler *= RADTODEG;
}

btVector3 getForceForward(btRigidBody* body, ShipComponent* ship)
{
	return getRigidBodyForward(body) * ship->forwardThrust;
}
btVector3 getForceAfterburner(btRigidBody* body, ShipComponent* ship)
{
	return getRigidBodyForward(body) * ship->afterburnerThrust;
}
btVector3 getForceBackward(btRigidBody* body, ShipComponent* ship)
{
	return getRigidBodyBackward(body) * ship->brakeThrust;
}
btVector3 getForceRight(btRigidBody* body, ShipComponent* ship)
{
	return getRigidBodyRight(body) * ship->strafeThrust;
}
btVector3 getForceLeft(btRigidBody* body, ShipComponent* ship)
{
	return getRigidBodyLeft(body) * ship->strafeThrust;
}
btVector3 getForceUp(btRigidBody* body, ShipComponent* ship)
{
	return getRigidBodyUp(body) * ship->strafeThrust;
}
btVector3 getForceDown(btRigidBody* body, ShipComponent* ship)
{
	return getRigidBodyDown(body) * ship->strafeThrust;
}
btVector3 getTorquePitchUp(btRigidBody* body, ShipComponent* ship)
{
	return getRigidBodyLeft(body) * ship->pitchThrust;
}
btVector3 getTorquePitchDown(btRigidBody* body, ShipComponent* ship)
{
	return getRigidBodyRight(body) * ship->pitchThrust;
}
btVector3 getTorqueYawRight(btRigidBody* body, ShipComponent* ship)
{
	return getRigidBodyUp(body) * ship->yawThrust;
}
btVector3 getTorqueYawLeft(btRigidBody* body, ShipComponent* ship)
{
	return getRigidBodyDown(body) * ship->yawThrust;
}
btVector3 getTorqueRollRight(btRigidBody* body, ShipComponent* ship)
{
	return getRigidBodyBackward(body) * ship->rollThrust;
}
btVector3 getTorqueRollLeft(btRigidBody* body, ShipComponent* ship)
{
	return getRigidBodyForward(body) * ship->rollThrust;
}

btVector3 velocitySafeNormalize(btVector3& velocity)
{
	btVector3 retval(velocity);
	if (retval.length2() <= 0) return btVector3(0, 0, 0);
	if (retval.length2() <= DEGENERATE_VECTOR_LENGTH) return retval;
	retval.normalize();
	return retval;
}

btVector3 getTorqueToStopAngularVelocity(btRigidBody* body, ShipComponent* ship)
{
	btVector3 ang = body->getAngularVelocity();
	ang = velocitySafeNormalize(ang);
	if (ang.length2() <= DEGENERATE_VECTOR_LENGTH) return -ang;
	return -ang * ((ship->pitchThrust + ship->yawThrust + ship->rollThrust) / 3.f);
}
btVector3 getForceToStopLinearVelocity(btRigidBody* body, ShipComponent* ship)
{
	btVector3 lin = body->getLinearVelocity();
	lin = velocitySafeNormalize(lin);
	if (lin.length2() <= DEGENERATE_VECTOR_LENGTH) return -lin;
	return -lin * (ship->brakeThrust + ship->strafeThrust);
}

void turnToDirection(btRigidBody* body, ShipComponent* ship, btVector3 dir)
{
	btVector3 right = getRigidBodyRight(body);
	btVector3 left = getRigidBodyLeft(body);
	btVector3 up = getRigidBodyUp(body);
	btVector3 down = getRigidBodyDown(body);

	if (right.dot(dir) > left.dot(dir)) {
		ship->moves[SHIP_YAW_RIGHT] = true;
	} else {
		ship->moves[SHIP_YAW_LEFT] = true;
	}
	if (up.dot(dir) > down.dot(dir)) {
		ship->moves[SHIP_PITCH_UP] = true;
	} else {
		ship->moves[SHIP_PITCH_DOWN] = true;
	}
}

void smoothTurnToDirection(btRigidBody* body, ShipComponent* ship, btVector3 dir)
{
	btScalar angle = getRigidBodyForward(body).angle(dir);
	btVector3 ang = body->getAngularVelocity();
	if (angle > ang.length()) {
		turnToDirection(body, ship, dir);
	} else {
		ship->moves[SHIP_STOP_ROTATION] = true;
	}
}

void goToPoint(btRigidBody* body, ShipComponent* ship, btVector3 dest, f32 dt)
{
	btVector3 shipPos = body->getCenterOfMassPosition();
	btVector3 path = dest - shipPos;
	btVector3 dir = path.normalized();
	btScalar angle = getRigidBodyForward(body).angle(dir) * RADTODEG;
	smoothTurnToDirection(body, ship, dir);
	if (angle < 20.f) {
		btScalar brakePower = ship->brakeThrust + ship->strafeThrust;
		btVector3 velocity = body->getLinearVelocity();
		btScalar timeToStop = velocity.length() / brakePower; //time required to stop in seconds
		btScalar timeToArrive = path.length() / velocity.length(); //time to arrive based on the current path
		if (timeToStop >= timeToArrive) { //You ever just write something so simple that you don't understand why it was such a PITA to get correct?

			ship->moves[SHIP_STOP_VELOCITY] = true;
		} else {
			ship->moves[SHIP_THRUST_FORWARD] = true;
		}
	}
	else {
		ship->moves[SHIP_STOP_VELOCITY] = true;
	}
}

bool avoidObstacles(flecs::entity id, f32 dt, flecs::entity target)
{
	if (!id.has<ShipComponent>() || !id.has<BulletRigidBodyComponent>() || !id.has<IrrlichtComponent>()) return false;

	auto ship = id.get_mut<ShipComponent>();
	auto rbc = id.get_mut<BulletRigidBodyComponent>();
	auto irr = id.get_mut<IrrlichtComponent>();

	btRigidBody* body = &rbc->rigidBody;

	btVector3 velocity = body->getLinearVelocity();
	btVector3 dir = velocitySafeNormalize(velocity);
	btVector3 pos = rbc->rigidBody.getCenterOfMassPosition();
	btVector3 futurePos = pos + (velocity *3.f); //where will it be three seconds from now
#if _DEBUG
	stateController->addDebugLine(line3df(btVecToIrr(pos), btVecToIrr(futurePos)));
#endif 
	auto cb = btClosestNotMeConvexResultCallback(body, pos, futurePos, bWorld->getPairCache(), bWorld->getDispatcher());
	auto shape = btSphereShape(3.f);
	btTransform from(body->getOrientation(), pos);
	btTransform to(body->getOrientation(), futurePos);
	to.setOrigin(futurePos);
	bWorld->convexSweepTest(&shape, from, to, cb);
	if (!cb.hasHit()) return false;
	//std::cout << "AAAAAA I'M GOING TO CRASH \n";
	ship->moves[SHIP_STOP_VELOCITY] = true;
	goToPoint(body, ship, (pos + getRigidBodyLeft(body) * 100.f), dt);

	return true;
}