#include "btUtils.h"

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

#if _DEBUG
void btDebugRenderer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	controller->addDebugLine(line3df(btVecToIrr(from), btVecToIrr(to)));
}
#endif 

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