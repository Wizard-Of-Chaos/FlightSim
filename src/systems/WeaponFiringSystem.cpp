#include "WeaponFiringSystem.h"
#include "GameController.h"
#include "SceneManager.h"
#include "IrrlichtUtils.h"
#include <iostream>

void fireAndReload(EntityId id, WeaponInfoComponent* wep, f32 dt)
{
	auto irr = sceneManager->scene.get<IrrlichtComponent>(id);
	auto kinInfo = sceneManager->scene.get<KineticInfoComponent>(id);
	if (kinInfo->clip > 0) {
		gameController->registerSoundInstance(id, sceneManager->defaults.defaultGunSound);
		createProjectileEntity(wep->spawnPosition, wep->firingDirection, id);
		kinInfo->clip -= 1;
		kinInfo->timeReloading = 0;
	}
	else {
		if (kinInfo->timeReloading >= kinInfo->reloadTime && kinInfo->ammunition > 0) {
			kinInfo->timeReloading = 0;
			kinInfo->clip = kinInfo->maxClip;
			kinInfo->ammunition -= kinInfo->maxClip;
		}
	}
}

void weaponFiringSystem(f32 dt)
{
	Scene* scene = &sceneManager->scene;

	for (auto entityId : SceneView<WeaponInfoComponent, IrrlichtComponent>(sceneManager->scene)) { //If the gun is firing, update time since last shot, play a sound and make the entity
		auto wepInfo = scene->get<WeaponInfoComponent>(entityId);
		if (wepInfo->type == WEP_NONE) continue;
		auto irrComp = scene->get<IrrlichtComponent>(entityId);

		if (wepInfo->type == WEP_KINETIC) {
			auto kinInfo = sceneManager->scene.get<KineticInfoComponent>(entityId);
			if (kinInfo->clip == 0) {
				kinInfo->timeReloading += dt;
			}
		}
		if (wepInfo->isFiring && (wepInfo->timeSinceLastShot > wepInfo->firingSpeed)) {
			if (wepInfo->type == WEP_KINETIC) {
				fireAndReload(entityId, wepInfo, dt);
			}
			else {
				gameController->registerSoundInstance(entityId, sceneManager->defaults.defaultLaserSound);
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
