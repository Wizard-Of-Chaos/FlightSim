#include "ProjectileSystem.h"
#include "GameController.h"
#include <iostream>

void projectileSystem(flecs::iter it, BulletRigidBodyComponent* rbcs, ProjectileInfoComponent* pic, IrrlichtComponent* irrs)
{
	for (auto i : it) {
		auto proj = &pic[i];
		auto irr = &irrs[i];
		auto rbc = &rbcs[i];
		auto ent = it.entity(i);
		proj->currentLifetime += it.delta_time();
		if (proj->currentLifetime >= proj->lifetime) {
			destroyProjectile(ent);
		}

		if (!ent.has<MissileProjectileComponent>()) continue;
		missileGoTo(ent, it.delta_time()); //need to change this to be managed better later
	}
}
/*
void projectileSystem(f32 dt)
{
	for (auto id : SceneView<BulletRigidBodyComponent, ProjectileInfoComponent, IrrlichtComponent>(sceneManager->scene)) {
		auto projectileInfo = sceneManager->scene.get<ProjectileInfoComponent>(id);
		auto irr = sceneManager->scene.get<IrrlichtComponent>(id);
		auto rbc = sceneManager->scene.get<BulletRigidBodyComponent>(id);
		projectileInfo->currentLifetime += dt;
		if (projectileInfo->currentLifetime >= projectileInfo->lifetime) {
			destroyProjectile(id);
		}

		auto miss = sceneManager->scene.get<MissileProjectileComponent>(id);
		if (miss) {
			missileGoTo(id, dt);
			//handle missile management
		}
	}
}
*/