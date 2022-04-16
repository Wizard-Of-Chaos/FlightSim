#include "ProjectileSystem.h"
#include "GameController.h"
#include "SceneManager.h"
#include <iostream>

void projectileSystem(f32 dt)
{
	for (auto id : SceneView<BulletRigidBodyComponent, ProjectileInfoComponent, IrrlichtComponent>(sceneManager->scene)) {
		auto projectileInfo = sceneManager->scene.get<ProjectileInfoComponent>(id);
		auto irr = sceneManager->scene.get<IrrlichtComponent>(id);
		auto rbc = sceneManager->scene.get<BulletRigidBodyComponent>(id);
		vector3df distance = irr->node->getAbsolutePosition() - projectileInfo->startPos;
		if (distance.getLength() >= projectileInfo->range) {
			destroyProjectile(id);
		}

		auto miss = sceneManager->scene.get<MissileProjectileComponent>(id);
		if (miss) {
			missileGoTo(id, dt);
			//handle missile management
		}
	}
}