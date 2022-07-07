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