#include "ProjectileSystem.h"
#include "GameController.h"
#include <iostream>

void projectileRangeSystem(SceneManager* manager, f32 dt)
{
	for (auto id : SceneView<BulletRigidBodyComponent, ProjectileInfoComponent, IrrlichtComponent>(manager->scene)) {
		auto projectileInfo = manager->scene.get<ProjectileInfoComponent>(id);
		auto irr = manager->scene.get<IrrlichtComponent>(id);
		vector3df distance = irr->node->getAbsolutePosition() - projectileInfo->startPos;
		if (distance.getLength() >= projectileInfo->range) {
			destroyProjectile(manager, id);
		}

		auto miss = manager->scene.get<MissileProjectileComponent>(id);
		if (miss) {
			if (!manager->scene.entityInUse(miss->target) || miss->target == INVALID_ENTITY) {
				explode(manager, irr->node->getPosition(), .3f);
				destroyProjectile(manager, id);
			}
			auto rbc = manager->scene.get<BulletRigidBodyComponent>(id);
			auto targetrbc = manager->scene.get<BulletRigidBodyComponent>(miss->target);
			missileGoTo(&rbc->rigidBody, projectileInfo, miss, targetrbc->rigidBody.getCenterOfMassPosition(), dt);
		}
	}
}