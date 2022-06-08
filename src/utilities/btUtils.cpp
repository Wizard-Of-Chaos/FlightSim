#include "btUtils.h"
#include "GameController.h"

bool initializeBtRigidBody(flecs::entity entityId, btConvexHullShape shape, btVector3& scale, f32 mass)
{
	if (!entityId.has<IrrlichtComponent>()) return false;

	auto objIrr = entityId.get<IrrlichtComponent>(entityId);

	BulletRigidBodyComponent* rbc = entityId.get_mut<BulletRigidBodyComponent>();
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

	setIdOnBtObject(&rbc->rigidBody, entityId);

	bWorld->addRigidBody(&(rbc->rigidBody));

	return true;
}

flecs::entity getIdFromBt(btCollisionObject* object)
{
	uint32_t bottom = object->getUserIndex();
	uint32_t top = object->getUserIndex2();

	flecs::entity_t id = ((flecs::entity_t)top << 32) | ((flecs::entity_t)bottom);

	flecs::entity ent(game_world->get_world(), id);
	return ent;
}

void setIdOnBtObject(btCollisionObject* object, flecs::entity id)
{
	size_t mask = ((1 << 32) - 1);
	uint32_t bottom = id.id() & mask;
	uint32_t top = (id.id() >> 32) & mask;
	object->setUserIndex(bottom);
	object->setUserIndex2(top);
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