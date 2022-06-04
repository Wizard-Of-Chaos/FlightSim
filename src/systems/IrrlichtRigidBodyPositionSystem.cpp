#include "IrrlichtRigidBodyPositionSystem.h"
#include <iostream>

void irrlichtRigidBodyPositionSystem(flecs::iter it, BulletRigidBodyComponent* rbcs, IrrlichtComponent* irrc)
{
	for (auto i : it) {
		BulletRigidBodyComponent* rbc = &rbcs[i];
		IrrlichtComponent* irr = &irrc[i];

		irr->node->setPosition(btVecToIrr(rbc->rigidBody.getWorldTransform().getOrigin()));
		btVector3 eulerOrientation;
		QuaternionToEuler(rbc->rigidBody.getOrientation(), eulerOrientation);
		irr->node->setRotation(btVecToIrr(eulerOrientation));
	}
}

/*
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
*/