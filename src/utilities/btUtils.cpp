#include "btUtils.h"
#include "SceneManager.h"
#include "GameController.h"

bool initializeBtRigidBody(EntityId entityId, btConvexHullShape shape, btVector3& scale, f32 mass)
{
	Scene* scene = &sceneManager->scene;

	auto objIrr = scene->get<IrrlichtComponent>(entityId);

	if (!objIrr) return false;

	BulletRigidBodyComponent* rbc = scene->assign<BulletRigidBodyComponent>(entityId);
	rbc->shape = shape;
	rbc->shape.setLocalScaling(scale);
	btTransform transform = btTransform();
	transform.setIdentity();
	transform.setOrigin(irrVecToBt(objIrr->node->getPosition()));
	btQuaternion q;
	btVector3 rot = irrVecToBt(objIrr->node->getRotation());
	q.setEuler(rot.y(), rot.x(), rot.z());
	transform.setRotation(q);
	auto motionState = new btDefaultMotionState(transform);

	btVector3 localInertia;
	rbc->shape.calculateLocalInertia(mass, localInertia);
	rbc->rigidBody = btRigidBody(mass, motionState, &rbc->shape, localInertia);
	rbc->rigidBody.setSleepingThresholds(0, 0);

	rbc->rigidBody.setUserIndex(getEntityIndex(entityId));
	rbc->rigidBody.setUserIndex2(getEntityVersion(entityId));
	rbc->rigidBody.setUserIndex3(1);

	bWorld->addRigidBody(&(rbc->rigidBody));

	return true;
}

EntityId getIdFromBt(btCollisionObject* object)
{
	EntityIndex ind = object->getUserIndex();
	EntityVersion ver = object->getUserIndex2();
	int hasEnt = object->getUserIndex3();
	EntityId id = INVALID_ENTITY;
	if (hasEnt) {
		id = createEntityId(ind, ver);
	}
	return id;
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

btVector3 getLocalAngularVelocity(btRigidBody* body)
{
	return body->getAngularVelocity() * body->getWorldTransform().getBasis().transpose();
}

btVector3 getLocalLinearVelocity(btRigidBody* body)
{
	return body->getLinearVelocity() * body->getWorldTransform().getBasis().transpose();
}

#if _DEBUG
void btDebugRenderer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	controller->addDebugLine(line3df(btVecToIrr(from), btVecToIrr(to)));
}
#endif 