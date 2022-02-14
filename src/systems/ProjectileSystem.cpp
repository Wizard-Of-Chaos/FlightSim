#include "ProjectileSystem.h"
#include "GameController.h"
#include <iostream>

void projectileRangeSystem(SceneManager* manager)
{
	for (auto id : SceneView<BulletRigidBodyComponent, ProjectileInfoComponent, IrrlichtComponent>(manager->scene)) {
		auto projectileInfo = manager->scene.get<ProjectileInfoComponent>(id);
		auto irr = manager->scene.get<IrrlichtComponent>(id);
		vector3df distance = irr->node->getAbsolutePosition() - projectileInfo->startPos;
		if (distance.getLength() >= projectileInfo->range) {
			destroyProjectile(manager, id);
		}
	}
}