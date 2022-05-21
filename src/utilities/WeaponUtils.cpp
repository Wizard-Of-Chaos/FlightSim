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

void handleSpecialWepFunctions(EntityId wep)
{

}

void gravityBolasHandler(EntityId wep)
{

}