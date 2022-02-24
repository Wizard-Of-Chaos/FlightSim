#include "WeaponFiringSystem.h"
#include "GameController.h"
#include "SceneManager.h"
#include "IrrlichtUtils.h"

void weaponFiringSystem(SceneManager* manager, f32 dt)
{
	Scene* scene = &manager->scene;
	for (auto entityId : SceneView<InputComponent, ShipComponent>(manager->scene)) {
		auto input = scene->get<InputComponent>(entityId);
		auto ship = scene->get<ShipComponent>(entityId);
		if (input->leftMouseDown) {
			for (unsigned int i = 0; i < ship->hardpointCount; ++i) {
				EntityId wep = ship->weapons[i];
				auto wepInfo = scene->get<WeaponInfoComponent>(wep);
				wepInfo->isFiring = true;
			}
		}
		else {
			for (unsigned int i = 0; i < ship->hardpointCount; ++i) {
				EntityId wep = ship->weapons[i];
				auto wepInfo = scene->get<WeaponInfoComponent>(wep);
				wepInfo->isFiring = false;
			}
		}
	}

	for (auto entityId : SceneView<WeaponInfoComponent, IrrlichtComponent>(manager->scene)) {
		auto wepInfo = scene->get<WeaponInfoComponent>(entityId);
		auto irrComp = scene->get<IrrlichtComponent>(entityId);

		vector3df spawnPos = irrComp->node->getAbsolutePosition() + (getNodeForward(irrComp->node) * 15.f);

		if (wepInfo->isFiring && (wepInfo->timeSinceLastShot > wepInfo->firingSpeed)) {
			manager->controller->soundEngine->play3D(manager->defaults.defaultLaserSound, spawnPos);
			createProjectileEntity(manager, spawnPos, getNodeForward(irrComp->node), entityId);
			wepInfo->timeSinceLastShot = 0.f;
		}
		wepInfo->timeSinceLastShot += dt;
		if (wepInfo->timeSinceLastShot > wepInfo->firingSpeed) {
			wepInfo->timeSinceLastShot = wepInfo->firingSpeed + .005f;
		}
	}
}
