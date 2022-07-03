#include "IrrlichtRigidBodyPositionSystem.h"
#include <iostream>

void irrlichtRigidBodyPositionSystem(flecs::iter it, BulletRigidBodyComponent* rbcs, IrrlichtComponent* irrc)
{
	for (auto i : it) {
		auto ent = it.entity(i);
		if (!ent.is_alive()) return;
		BulletRigidBodyComponent* rbc = &rbcs[i];
		IrrlichtComponent* irr = &irrc[i];
		if (!rbc->rigidBody) {
			std::cout << "AAAAAA THERE'S NO RIGID BODY \n";
			continue;
		}
		else if (!irr->node) {
			std::cout << "AAAAAA THERE'S NO NODE \n";
			continue;
		}
		irrc[i].node->setPosition(btVecToIrr(rbcs[i].rigidBody->getWorldTransform().getOrigin()));
		btVector3 eulerOrientation;
		QuaternionToEuler(rbcs[i].rigidBody->getOrientation(), eulerOrientation);
		irrc[i].node->setRotation(btVecToIrr(eulerOrientation));
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