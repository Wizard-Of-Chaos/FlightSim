#include "WeaponUtils.h"
#include "GameController.h"
#include "GameStateController.h"

void handleProjectileImpact(flecs::entity projectile, flecs::entity impacted)
{
	auto proj = projectile.get<ProjectileInfoComponent>();

	switch (proj->type) {
	case WEP_MISSILE:
		missileImpact(projectile);
		break;
	case WEP_PHYS_IMPULSE:
		impulseBlasterImpact(projectile, impacted);
		break;
	case WEP_PHYS_BOLAS:
		gravityBolasImpact(projectile, impacted);
		break;
	default:
		break;
	}
}

void impulseBlasterImpact(flecs::entity projId, flecs::entity impacted)
{
	auto proj = projId.get<ProjectileInfoComponent>();
	auto irr = projId.get<IrrlichtComponent>();

	gameController->registerSoundInstance(impacted, stateController->assets.getSoundAsset("physicsBlastSound"), 1.f, 200.f);
	explode(irr->node->getAbsolutePosition(), 1.f, 1.f, 80.f, proj->damage, 500.f);
}

void missileImpact(flecs::entity projId)
{
	auto proj = projId.get<ProjectileInfoComponent>();
	auto irr = projId.get<IrrlichtComponent>();

	explode(irr->node->getAbsolutePosition(), 1.f, 1.f, 20.f, proj->damage, 100.f);
}

void gravityBolasImpact(flecs::entity projId, flecs::entity impacted)
{
	flecs::entity weapon = projId.get_object<FiredBy>();
	if (!weapon.is_alive()) return;
	if (!weapon.has<BolasInfoComponent>()) return;

	auto bolasInfo = weapon.get_mut<BolasInfoComponent>();

	if (bolasInfo->target1.is_alive() && bolasInfo->target2.is_alive()) return; //one set of targets at a time

	if (!bolasInfo->target1.is_alive()) {
		bolasInfo->target1 = impacted;
		std::cout << "Bolas target 1 locked\n";
		gameController->registerSoundInstance(impacted, stateController->assets.getSoundAsset("bolasHitSound"), .5f, 100.f);
	}
	else if (!bolasInfo->target2.is_alive()) {
		if (bolasInfo->target1 == impacted) return;
		bolasInfo->target2 = impacted;
		std::cout << "Bolas target 2 locked\n";


		if (!bolasInfo->target1.has<BulletRigidBodyComponent>() || !bolasInfo->target2.has<BulletRigidBodyComponent>()) {
			bolasInfo->target1 = INVALID_ENTITY;
			bolasInfo->target2 = INVALID_ENTITY;
			return;
		}
		auto rbcA = bolasInfo->target1.get_mut<BulletRigidBodyComponent>();
		auto rbcB = bolasInfo->target2.get_mut<BulletRigidBodyComponent>();

		gameController->registerSoundInstance(impacted, stateController->assets.getSoundAsset("bolasLatchSound"), 1.f, 100.f);

		btTransform tr;
		btVector3 ori(0, 0, 0);
		tr.setIdentity();
		tr.setOrigin(ori);
		auto p2p = new btGeneric6DofConstraint(rbcA->rigidBody, rbcB->rigidBody, tr, tr, false);
		p2p->setLinearLowerLimit(btVector3(0, 0, 0));
		p2p->setLinearUpperLimit(btVector3(0, 0, 0));
		p2p->setAngularLowerLimit(btVector3(-PI, -PI, -PI));
		p2p->setAngularUpperLimit(btVector3(PI, PI, PI));
		for (u32 i = 0; i < 6; ++i) {
			f32 forceFactor = .005f * bolasInfo->force;
			p2p->setParam(BT_CONSTRAINT_STOP_ERP, forceFactor, i);
		}
		bolasInfo->constraint = p2p;
		bWorld->addConstraint(bolasInfo->constraint);
	}
}

void handleSpecialWepFunctions(flecs::entity wep, f32 dt)
{
	if (!wep.has<WeaponInfoComponent>()) return;

	auto wepInfo = wep.get<WeaponInfoComponent>();

	switch (wepInfo->type) {
	case WEP_PHYS_BOLAS:
		gravityBolasHandler(wep, dt);
		break;
	default:
		break;
	}
}

void gravityBolasHandler(flecs::entity wep, f32 dt)
{
	if (!wep.has<BolasInfoComponent>() || !wep.is_alive()) return;
	auto bolasInfo = wep.get_mut<BolasInfoComponent>();
	if (!bolasInfo->target1.is_alive()) {
		bolasInfo->target2 = INVALID_ENTITY;
		return;
	}
	if (bolasInfo->target1.is_alive() && !bolasInfo->target2.is_alive()) {
		bolasInfo->currentTimeToHit += dt;
		if (bolasInfo->currentTimeToHit >= bolasInfo->timeToHit) {
			bolasInfo->target1 = INVALID_ENTITY;
			bolasInfo->target2 = INVALID_ENTITY;
			bolasInfo->currentTimeToHit = 0;
			return;
		}
	}
	else if (bolasInfo->target1.is_alive() && !bolasInfo->target2.is_alive()) {
		bolasInfo->currentDuration += dt;
		if (bolasInfo->currentDuration >= bolasInfo->duration) {
			bolasInfo->target1 = INVALID_ENTITY;
			bolasInfo->target2 = INVALID_ENTITY;
			bolasInfo->currentDuration = 0;
			if (bolasInfo->constraint) {
				bWorld->removeConstraint(bolasInfo->constraint);
				delete bolasInfo->constraint;
			}
			bolasInfo->constraint = nullptr;
			return;
		}
	}
}