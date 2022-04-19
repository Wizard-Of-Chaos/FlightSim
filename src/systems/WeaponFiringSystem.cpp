#include "WeaponFiringSystem.h"
#include "GameController.h"
#include "GameStateController.h"
#include "SceneManager.h"
#include "IrrlichtUtils.h"

#include <iostream>

bool ammoFire(EntityId id, WeaponInfoComponent* wep, f32 dt)
{
	auto irr = sceneManager->scene.get<IrrlichtComponent>(id);
	if (wep->clip > 0) {
		createProjectileEntity(wep->spawnPosition, wep->firingDirection, id);
		wep->clip -= 1;
		wep->timeReloading = 0;
		return true;
	}
	else {
		if (wep->timeReloading >= wep->reloadTime && wep->ammunition > 0) {
			wep->timeReloading = 0;
			wep->clip = wep->maxClip;
			wep->ammunition -= wep->maxClip;
			return false;
		}
	}
	return true;
}

void weaponFiringSystem(f32 dt)
{
	Scene* scene = &sceneManager->scene;

	for (auto entityId : SceneView<WeaponInfoComponent, IrrlichtComponent>(sceneManager->scene)) { //If the gun is firing, update time since last shot, play a sound and make the entity
		auto wepInfo = scene->get<WeaponInfoComponent>(entityId);
		if (wepInfo->type == WEP_NONE) continue;
		auto irrComp = scene->get<IrrlichtComponent>(entityId);

		if (wepInfo->usesAmmunition) {
			if (wepInfo->clip == 0) {
				wepInfo->timeReloading += dt;
			}
		}
		if (wepInfo->isFiring && (wepInfo->timeSinceLastShot > wepInfo->firingSpeed)) {
			if (wepInfo->usesAmmunition) {
				bool firing = ammoFire(entityId, wepInfo, dt);
				if(wepInfo->type == WEP_KINETIC && firing) gameController->registerSoundInstance(entityId, stateController->assets.getSoundAsset("gunSound"), .3f, 10.f);
			}
			else {
				gameController->registerSoundInstance(entityId, stateController->assets.getSoundAsset("laserSound"), .7f, 10.f);
				createProjectileEntity(wepInfo->spawnPosition, wepInfo->firingDirection, entityId);
			}
			wepInfo->timeSinceLastShot = 0.f;
		}
		wepInfo->timeSinceLastShot += dt;
		if (wepInfo->timeSinceLastShot > wepInfo->firingSpeed) {
			wepInfo->timeSinceLastShot = wepInfo->firingSpeed + .005f;
		}
	}
}
