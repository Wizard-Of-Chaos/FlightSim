#include "IrrlichtRigidBodyPositionSystem.h"
#include <iostream>

void irrlichtRigidBodyPositionSystem(Scene& scene, f32 dt)
{
	for (auto entityId : SceneView<BulletRigidBodyComponent, IrrlichtComponent>(scene)) {

		BulletRigidBodyComponent* rbc = scene.get<BulletRigidBodyComponent>(entityId);
		IrrlichtComponent* irrcomp = scene.get<IrrlichtComponent>(entityId);

		irrcomp->node->setPosition(btVecToIrr(rbc->rigidBody.getWorldTransform().getOrigin()));
		btVector3 eulerOrientation;
		QuaternionToEuler(rbc->rigidBody.getOrientation(), eulerOrientation);
		irrcomp->node->setRotation(btVecToIrr(eulerOrientation));
	}
}