#include "ShipMovementUtils.h"
#include <iostream>

void QuaternionToEuler(const btQuaternion& TQuat, btVector3& TEuler) {
	btScalar x, y, z;

	TQuat.getEulerZYX(z, y, x);
	TEuler.setValue(x, y, z);
	TEuler *= RADTODEG;
}

btVector3 getRigidBodyForward(btRigidBody* body)
{
	btVector3 forward(0, 0, 1);
	btQuaternion transRot = body->getCenterOfMassTransform().getRotation();
	return forward.rotate(transRot.getAxis(), transRot.getAngle());
}
btVector3 getRigidBodyBackward(btRigidBody* body)
{
	return -getRigidBodyForward(body);
}
btVector3 getRigidBodyRight(btRigidBody* body)
{
	btVector3 right(1, 0, 0);
	btQuaternion transRot = body->getCenterOfMassTransform().getRotation();
	return right.rotate(transRot.getAxis(), transRot.getAngle());
}
btVector3 getRigidBodyLeft(btRigidBody* body)
{
	return -getRigidBodyRight(body);
}
btVector3 getRigidBodyUp(btRigidBody* body)
{
	btVector3 up(0, 1, 0);
	btQuaternion transRot = body->getCenterOfMassTransform().getRotation();
	return up.rotate(transRot.getAxis(), transRot.getAngle());
}
btVector3 getRigidBodyDown(btRigidBody* body)
{
	return -getRigidBodyUp(body);
}

btVector3 getForceForward(btRigidBody* body, ShipComponent* ship)
{
	return getRigidBodyForward(body) * ship->forwardThrust;
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

btVector3 getTorqueToStopAngularVelocity(btRigidBody* body, ShipComponent* ship)
{
	btVector3 ang = body->getAngularVelocity();
	if (ang.length2() <= 0) return btVector3(0, 0, 0);
	if (ang.length2() <= 0.00001f) return -ang;
	ang.normalize();
	return -ang * ((ship->pitchThrust + ship->yawThrust + ship->rollThrust) / 3.f);
}
btVector3 getForceToStopLinearVelocity(btRigidBody* body, ShipComponent* ship)
{
	btVector3 lin = body->getLinearVelocity();
	if (lin.length2() <= 0) return btVector3(0, 0, 0);
	if (lin.length2() <= 0.00001f) return -lin;
	lin.normalize();
	return -lin * (ship->brakeThrust + ship->strafeThrust);
}

btVector3 getTorqueToDirection(btRigidBody* body, ShipComponent* ship, btVector3 dir)
{
	btVector3 velocity = body->getAngularVelocity();

	btVector3 forward = getRigidBodyForward(body);
	btVector3 right = getRigidBodyRight(body);
	btVector3 left = getRigidBodyLeft(body);
	btVector3 up = getRigidBodyUp(body);
	btVector3 down = getRigidBodyDown(body);

	btScalar angle = forward.angle(dir) * RADTODEG;
	btScalar torqueFactor = (angle / 180.f);

	btVector3 torque(0, 0, 0);
	if (right.dot(dir) > left.dot(dir)) {
		torque += getTorqueYawRight(body, ship);
	}
	else {
		torque += getTorqueYawLeft(body, ship);
	}
	if (up.dot(dir) > down.dot(dir)) {
		torque += getTorquePitchUp(body, ship);
	}
	else {
		torque += getTorquePitchDown(body, ship);
	}
	torque *= torqueFactor;
	return torque;
}

btVector3 getTorqueOpposingDirection(btRigidBody* body, ShipComponent* ship, btVector3 dir)
{
	btVector3 forward = getRigidBodyForward(body);
	btScalar angle = forward.angle(dir) * RADTODEG;
	btScalar torqueFactor = ((180.f - angle) / 180.f);
	btVector3 dirTorque = -getTorqueToDirection(body, ship, dir);
	return dirTorque * torqueFactor;
}

void smoothTurnToDirection(btRigidBody* body, ShipComponent* ship, btVector3 dir, f32 dt)
{
	btVector3 torque = getTorqueToDirection(body, ship, dir);
	btVector3 opp = getTorqueOpposingDirection(body, ship, dir);
	btScalar velocityFactor = body->getAngularVelocity().length();
	torque /= velocityFactor * 3.f;
	opp /= velocityFactor;
	btScalar angle = getRigidBodyForward(body).angle(dir) * RADTODEG;
	if (angle > 5.f) {
		body->applyTorqueImpulse(getTorqueToDirection(body, ship, dir) * dt);
		body->applyTorqueImpulse(getTorqueOpposingDirection(body, ship, dir) * dt);
	}
	else {
		body->applyTorqueImpulse(getTorqueToStopAngularVelocity(body, ship) * dt);
	}
}

void goToPoint(btRigidBody* body, ShipComponent* ship, btVector3 dest, f32 dt)
{
	btVector3 shipPos = body->getCenterOfMassPosition();
	btVector3 path = dest - shipPos;
	btVector3 dir = path.normalized();
	btScalar angle = getRigidBodyForward(body).angle(dir) * RADTODEG;
	smoothTurnToDirection(body, ship, dir, dt);
	if (angle < 10.f) {
		if (path.length() <= body->getAngularVelocity().length()) {
			body->applyCentralImpulse(getForceToStopLinearVelocity(body, ship) * dt);
		}
		else {
			body->applyCentralImpulse(getForceForward(body, ship) * dt);
		}
	}
}