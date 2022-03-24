#include "ProjectileSystem.h"
#include "GameController.h"
#include <iostream>

void projectileRangeSystem(SceneManager* manager, f32 dt)
{
	for (auto id : SceneView<BulletRigidBodyComponent, ProjectileInfoComponent, IrrlichtComponent>(manager->scene)) {
		auto projectileInfo = manager->scene.get<ProjectileInfoComponent>(id);
		auto irr = manager->scene.get<IrrlichtComponent>(id);
		auto rbc = manager->scene.get<BulletRigidBodyComponent>(id);
		vector3df distance = irr->node->getAbsolutePosition() - projectileInfo->startPos;
		if (distance.getLength() >= projectileInfo->range) {
			destroyProjectile(manager, id);
		}

		auto miss = manager->scene.get<MissileProjectileComponent>(id);
		if (miss) {
			missileGoTo(manager, id, dt);
			//handle missile management
		}
	}
}