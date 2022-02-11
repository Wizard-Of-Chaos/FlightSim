#include "ShipMovementUtils.h"
#include "SceneManager.h"
#include "GameController.h"

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

btVector3 velocitySafeNormalize(btVector3& velocity)
{
	btVector3 retval(velocity);
	if (retval.length2() <= 0) return btVector3(0, 0, 0);
	if (retval.length2() <= 0.00001f) return retval;
	retval.normalize();
	return retval;
}

btVector3 getTorqueToStopAngularVelocity(btRigidBody* body, ShipComponent* ship)
{
	btVector3 ang = body->getAngularVelocity();
	ang = velocitySafeNormalize(ang);
	if (ang.length2() <= 0.00001f) return -ang;
	return -ang * ((ship->pitchThrust + ship->yawThrust + ship->rollThrust) / 3.f);
}
btVector3 getForceToStopLinearVelocity(btRigidBody* body, ShipComponent* ship)
{
	btVector3 lin = body->getLinearVelocity();
	lin = velocitySafeNormalize(lin);
	if (lin.length2() <= 0.00001f) return -lin;
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
	//btScalar torqueFactor = (angle / 180.f);

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
	//torque *= torqueFactor;
	return torque;
}

void smoothTurnToDirection(btRigidBody* body, ShipComponent* ship, btVector3 dir, f32 dt)
{
	btVector3 torque = getTorqueToDirection(body, ship, dir);
	btScalar angle = getRigidBodyForward(body).angle(dir);
	btVector3 ang = body->getAngularVelocity();
	if (angle > ang.length()) {
		body->applyTorqueImpulse(torque * dt);
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
	if (angle < 20.f) {
		btScalar brakePower = ship->brakeThrust + ship->strafeThrust;
		btVector3 velocity = body->getLinearVelocity();
		btScalar timeToStop = velocity.length() / brakePower; //time required to stop in seconds
		btScalar timeToArrive = path.length() / velocity.length(); //time to arrive based on the current path
		if (timeToStop >= timeToArrive) { //You ever just write something so simple that you don't understand why it was such a PITA to get correct?
			body->applyCentralImpulse(getForceToStopLinearVelocity(body, ship) * dt);
		}
		else {
			body->applyCentralImpulse(getForceForward(body, ship) * dt);
		}
	}
}

bool avoidObstacles(SceneManager* manager, EntityId id, f32 dt, EntityId target)
{
	ISceneCollisionManager* coll = manager->controller->smgr->getSceneCollisionManager();

	auto ship = manager->scene.get<ShipComponent>(id);
	auto rbc = manager->scene.get<BulletRigidBodyComponent>(id);
	auto irr = manager->scene.get<IrrlichtComponent>(id);

	if (!ship || !rbc) return false;
	btRigidBody* body = &rbc->rigidBody;

	btVector3 velocity = body->getLinearVelocity();
	btVector3 dir = velocitySafeNormalize(velocity);
	vector3df pos = irr->node->getPosition();
	vector3df futurePos = pos + (btVecToIrr(dir) * velocity.length() * 3.f);

	line3df trajectory(pos, futurePos);
	ISceneNode* obstacle = coll->getSceneNodeFromRayBB(trajectory, ID_IsAvoidable);

	if (obstacle) {
		EntityId obstacleId = strToId(std::string(obstacle->getName()));
		if (target != INVALID_ENTITY && obstacleId == target) return false;

		//now what?

		return true;
	}
	return false;
}