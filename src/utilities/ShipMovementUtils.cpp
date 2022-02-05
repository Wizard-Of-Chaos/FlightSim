#include "ShipMovementUtils.h"

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

btVector3 getTorqueOnAxis(ShipComponent* ship, btVector3 axis)
{
	return axis * ship->rotSpeed;
}
btVector3 getForceToDirection(ShipComponent* ship, btVector3 dir)
{
	return dir * ship->speed;
}

btVector3 getForceForward(btRigidBody* body, ShipComponent* ship)
{
	return getForceToDirection(ship, getRigidBodyForward(body));
}
btVector3 getForceBackward(btRigidBody* body, ShipComponent* ship)
{
	return getForceToDirection(ship, getRigidBodyBackward(body));
}
btVector3 getForceRight(btRigidBody* body, ShipComponent* ship)
{
	return getForceToDirection(ship, getRigidBodyRight(body));
}
btVector3 getForceLeft(btRigidBody* body, ShipComponent* ship)
{
	return getForceToDirection(ship, getRigidBodyLeft(body));
}
btVector3 getForceUp(btRigidBody* body, ShipComponent* ship)
{
	return getForceToDirection(ship, getRigidBodyUp(body));
}
btVector3 getForceDown(btRigidBody* body, ShipComponent* ship)
{
	return getForceToDirection(ship, getRigidBodyDown(body));
}

btVector3 getTorquePitchUp(btRigidBody* body, ShipComponent* ship)
{
	return getTorqueOnAxis(ship, getRigidBodyLeft(body));
}
btVector3 getTorquePitchDown(btRigidBody* body, ShipComponent* ship)
{
	return getTorqueOnAxis(ship, getRigidBodyRight(body));
}
btVector3 getTorqueYawRight(btRigidBody* body, ShipComponent* ship)
{
	return getTorqueOnAxis(ship, getRigidBodyUp(body));
}
btVector3 getTorqueYawLeft(btRigidBody* body, ShipComponent* ship)
{
	return getTorqueOnAxis(ship, getRigidBodyDown(body));
}
btVector3 getTorqueRollRight(btRigidBody* body, ShipComponent* ship)
{
	return getTorqueOnAxis(ship, getRigidBodyBackward(body));
}
btVector3 getTorqueRollLeft(btRigidBody* body, ShipComponent* ship)
{
	return getTorqueOnAxis(ship, getRigidBodyForward(body));
}

btVector3 getTorqueToStopAngularVelocity(btRigidBody* body, ShipComponent* ship)
{
	btVector3 ang = body->getAngularVelocity();
	ang.safeNormalize();
	return -ang * ship->rotSpeed;
}
btVector3 getForceToStopLinearVelocity(btRigidBody* body, ShipComponent* ship)
{
	btVector3 lin = body->getLinearVelocity();
	lin.safeNormalize();
	return -lin * ship->rotSpeed;
}

btVector3 getTorqueToFaceDirection(btRigidBody* body, ShipComponent* ship, btVector3 dir)
{
	btVector3 axis = dir.cross(getRigidBodyForward(body));
	btScalar angle = dir.angle(getRigidBodyForward(body));

	btVector3 euler;
	btQuaternion targetRot(axis, angle);
	QuaternionToEuler(targetRot, euler);

	return euler * ship->speed;
}