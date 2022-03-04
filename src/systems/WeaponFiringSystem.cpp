#include "WeaponFiringSystem.h"
#include "GameController.h"
#include "SceneManager.h"
#include "IrrlichtUtils.h"
#include <iostream>

void weaponFiringSystem(SceneManager* manager, f32 dt)
{
	Scene* scene = &manager->scene;

	for (auto entityId : SceneView<WeaponInfoComponent, IrrlichtComponent>(manager->scene)) { //If the gun is firing, update time since last shot, play a sound and make the entity
		auto wepInfo = scene->get<WeaponInfoComponent>(entityId);
		auto irrComp = scene->get<IrrlichtComponent>(entityId);

		if (wepInfo->isFiring && (wepInfo->timeSinceLastShot > wepInfo->firingSpeed)) {
			manager->controller->soundEngine->play3D(manager->defaults.defaultLaserSound, wepInfo->spawnPosition);
			createProjectileEntity(manager, wepInfo->spawnPosition, wepInfo->firingDirection, entityId);
			wepInfo->timeSinceLastShot = 0.f;
		}
		wepInfo->timeSinceLastShot += dt;
		if (wepInfo->timeSinceLastShot > wepInfo->firingSpeed) {
			wepInfo->timeSinceLastShot = wepInfo->firingSpeed + .005f;
		}
	}
}
