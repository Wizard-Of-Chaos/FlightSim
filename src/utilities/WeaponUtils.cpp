#include "WeaponUtils.h"
#include "GameController.h"
#include "GameStateController.h"
#include "SceneManager.h"

void handleProjectileImpact(EntityId projectile, EntityId impacted)
{
	auto proj = sceneManager->scene.get<ProjectileInfoComponent>(projectile);

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

void impulseBlasterImpact(EntityId projId, EntityId impacted)
{
	auto proj = sceneManager->scene.get<ProjectileInfoComponent>(projId);
	auto irr = sceneManager->scene.get<IrrlichtComponent>(projId);

	gameController->registerSoundInstance(impacted, stateController->assets.getSoundAsset("physicsBlastSound"), 1.f, 200.f);
	explode(irr->node->getAbsolutePosition(), 1.f, 1.f, 80.f, proj->damage, 500.f);
}

void missileImpact(EntityId projId)
{
	auto proj = sceneManager->scene.get<ProjectileInfoComponent>(projId);
	auto irr = sceneManager->scene.get<IrrlichtComponent>(projId);

	explode(irr->node->getAbsolutePosition(), 1.f, 1.f, 20.f, proj->damage, 100.f);
}

void gravityBolasImpact(EntityId projId, EntityId impacted)
{
	EntityId wepId = sceneManager->scene.get<ParentComponent>(projId)->parentId;

	auto bolasInfo = sceneManager->scene.get<BolasInfoComponent>(wepId);

	if (sceneManager->scene.entityInUse(bolasInfo->target1) && sceneManager->scene.entityInUse(bolasInfo->target2)) return; //One at a time, dammit!

	if (!sceneManager->scene.entityInUse(bolasInfo->target1)) {
		bolasInfo->target1 = impacted;
		std::cout << "Bolas target 1 locked\n";
		gameController->registerSoundInstance(impacted, stateController->assets.getSoundAsset("bolasHitSound"), .5f, 100.f);
	}
	else if (!sceneManager->scene.entityInUse(bolasInfo->target2)) {
		if (bolasInfo->target1 == impacted) return;
		bolasInfo->target2 = impacted;
		std::cout << "Bolas target 2 locked\n";

		auto rbcA = sceneManager->scene.get<BulletRigidBodyComponent>(bolasInfo->target1);
		auto rbcB = sceneManager->scene.get<BulletRigidBodyComponent>(bolasInfo->target2);

		if (!rbcA || !rbcB) {
			bolasInfo->target1 = INVALID_ENTITY;
			bolasInfo->target2 = INVALID_ENTITY;
			return;
		}
		gameController->registerSoundInstance(impacted, stateController->assets.getSoundAsset("bolasLatchSound"), 1.f, 100.f);
		/*
		auto p2p = new btPoint2PointConstraint(rbcA->rigidBody, rbcB->rigidBody, btVector3(0, 0, 0), btVector3(0, 0, 0));
		p2p->m_setting.m_damping = 0.0625f;
		p2p->m_setting.m_impulseClamp = 0.95f;
		p2p->setOverrideNumSolverIterations(200);
		*/
		btTransform tr;
		btVector3 ori(0, 0, 0);
		tr.setIdentity();
		tr.setOrigin(ori);
		auto p2p = new btGeneric6DofConstraint(rbcA->rigidBody, rbcB->rigidBody, tr, tr, false);
		p2p->setLinearLowerLimit(btVector3(0, 0, 0));
		p2p->setLinearUpperLimit(btVector3(0, 0, 0));
		for (u32 i = 0; i < 6; ++i) {
			//p2p->setParam(BT_CONSTRAINT_STOP_CFM, .005f, i);
			f32 forceFactor = .005f * bolasInfo->force;
			p2p->setParam(BT_CONSTRAINT_STOP_ERP, forceFactor, i);
		}
		bolasInfo->constraint = p2p;
		bWorld->addConstraint(bolasInfo->constraint);
	}
}

void handleSpecialWepFunctions(EntityId wep, f32 dt)
{
	auto wepInfo = sceneManager->scene.get<WeaponInfoComponent>(wep);
	switch (wepInfo->type) {
	case WEP_PHYS_BOLAS:
		gravityBolasHandler(wep, dt);
		break;
	default:
		break;
	}
}

void gravityBolasHandler(EntityId wep, f32 dt)
{
	auto bolasInfo = sceneManager->scene.get<BolasInfoComponent>(wep);
	if (!sceneManager->scene.entityInUse(bolasInfo->target1)) return;

	if (sceneManager->scene.entityInUse(bolasInfo->target1) && !sceneManager->scene.entityInUse(bolasInfo->target2)) {
		bolasInfo->currentTimeToHit += dt;
		if (bolasInfo->currentTimeToHit >= bolasInfo->timeToHit) {
			bolasInfo->target1 = INVALID_ENTITY;
			bolasInfo->target2 = INVALID_ENTITY;
			bolasInfo->currentTimeToHit = 0;
			return;
		}
	}
	else if (sceneManager->scene.entityInUse(bolasInfo->target1) && sceneManager->scene.entityInUse(bolasInfo->target2)) {
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