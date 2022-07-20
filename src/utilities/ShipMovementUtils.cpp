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

btVector3 getForceForward(btRigidBody* body, ThrustComponent* thrust)
{
	return getRigidBodyForward(body) * thrust->forward;
}
btVector3 getForceBoost(btRigidBody* body, ThrustComponent* thrust)
{
	return getRigidBodyForward(body) * thrust->boost;
}
btVector3 getForceBackward(btRigidBody* body, ThrustComponent* thrust)
{
	return getRigidBodyBackward(body) * thrust->brake;
}
btVector3 getForceRight(btRigidBody* body, ThrustComponent* thrust)
{
	return getRigidBodyRight(body) * thrust->strafe;
}
btVector3 getForceLeft(btRigidBody* body, ThrustComponent* thrust)
{
	return getRigidBodyLeft(body) * thrust->strafe;
}
btVector3 getForceUp(btRigidBody* body, ThrustComponent* thrust)
{
	return getRigidBodyUp(body) * thrust->strafe;
}
btVector3 getForceDown(btRigidBody* body, ThrustComponent* thrust)
{
	return getRigidBodyDown(body) * thrust->strafe;
}
btVector3 getTorquePitchUp(btRigidBody* body, ThrustComponent* thrust)
{
	return getRigidBodyLeft(body) * thrust->pitch;
}
btVector3 getTorquePitchDown(btRigidBody* body, ThrustComponent* thrust)
{
	return getRigidBodyRight(body) * thrust->pitch;
}
btVector3 getTorqueYawRight(btRigidBody* body, ThrustComponent* thrust)
{
	return getRigidBodyUp(body) * thrust->pitch;
}
btVector3 getTorqueYawLeft(btRigidBody* body, ThrustComponent* thrust)
{
	return getRigidBodyDown(body) * thrust->pitch;
}
btVector3 getTorqueRollRight(btRigidBody* body, ThrustComponent* thrust)
{
	return getRigidBodyBackward(body) * thrust->pitch;
}
btVector3 getTorqueRollLeft(btRigidBody* body, ThrustComponent* thrust)
{
	return getRigidBodyForward(body) * thrust->pitch;
}

btVector3 velocitySafeNormalize(btVector3& velocity)
{
	btVector3 retval(velocity);
	if (retval.length2() <= 0) return btVector3(0, 0, 0);
	if (retval.length2() <= DEGENERATE_VECTOR_LENGTH) return retval;
	retval.normalize();
	return retval;
}

btVector3 getTorqueToStopAngularVelocity(btRigidBody* body, ThrustComponent* thrust)
{
	btVector3 ang = body->getAngularVelocity();
	ang = velocitySafeNormalize(ang);
	if (ang.length2() <= DEGENERATE_VECTOR_LENGTH) return -ang;
	return -ang * ((thrust->pitch + thrust->yaw + thrust->roll) / 3.f);
}
btVector3 getForceToStopLinearVelocity(btRigidBody* body, ThrustComponent* thrust)
{
	btVector3 lin = body->getLinearVelocity();
	lin = velocitySafeNormalize(lin);
	if (lin.length2() <= DEGENERATE_VECTOR_LENGTH) return -lin;
	return -lin * (thrust->brake + thrust->brake);
}

void turnToDirection(btRigidBody* body, ThrustComponent* thrust, btVector3 dir)
{
	btVector3 right = getRigidBodyRight(body);
	btVector3 left = getRigidBodyLeft(body);
	btVector3 up = getRigidBodyUp(body);
	btVector3 down = getRigidBodyDown(body);
	if (right.dot(dir) > left.dot(dir)) {
		thrust->moves[YAW_RIGHT] = true;
	} else {
		thrust->moves[YAW_LEFT] = true;
	}
	if (up.dot(dir) > down.dot(dir)) {
		thrust->moves[PITCH_UP] = true;
	} else {
		thrust->moves[PITCH_DOWN] = true;
	}
}

void smoothTurnToDirection(btRigidBody* body, ThrustComponent* thrust, btVector3 dir)
{
	btScalar angle = getRigidBodyForward(body).angle(dir);
	btVector3 ang = body->getAngularVelocity();
	if (angle > ang.length()) {
		turnToDirection(body, thrust, dir);
	} else {
		thrust->moves[STOP_ROTATION] = true;
	}
}

void goToPoint(btRigidBody* body, ThrustComponent* thrust, btVector3 dest, f32 dt)
{
	btVector3 shipPos = body->getCenterOfMassPosition();
	btVector3 path = dest - shipPos;
	btVector3 dir = path.normalized();
	btScalar angle = getRigidBodyForward(body).angle(dir) * RADTODEG;
	smoothTurnToDirection(body, thrust, dir);
	if (angle < 20.f) {
		btScalar brakePower = thrust->brake + thrust->strafe;
		btVector3 velocity = body->getLinearVelocity();
		btScalar timeToStop = velocity.length() / brakePower; //time required to stop in seconds
		btScalar timeToArrive = path.length() / velocity.length(); //time to arrive based on the current path
		if (timeToStop >= timeToArrive) { //You ever just write something so simple that you don't understand why it was such a PITA to get correct?

			thrust->moves[STOP_VELOCITY] = true;
		} else {
			thrust->moves[THRUST_FORWARD] = true;
		}
	}
	else {
		thrust->moves[STOP_VELOCITY] = true;
	}
}

bool avoidObstacles(ThrustComponent* thrust, BulletRigidBodyComponent* rbc, IrrlichtComponent* irr, f32 dt, flecs::entity target)
{
	btRigidBody* body = rbc->rigidBody;

	btVector3 velocity = body->getLinearVelocity();
	btVector3 dir = velocitySafeNormalize(velocity);
	btVector3 pos = rbc->rigidBody->getCenterOfMassPosition();
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
	thrust->moves[STOP_VELOCITY] = true;
	goToPoint(body, thrust, (pos + getRigidBodyLeft(body) * 100.f), dt); //swerves left 

	return true;
}