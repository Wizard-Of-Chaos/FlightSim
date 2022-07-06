#include "WeaponFiringSystem.h"
#include "GameController.h"
#include "GameStateController.h"
#include "IrrlichtUtils.h"
#include "WeaponUtils.h"
#include <iostream>

bool ammoFire(flecs::entity id, WeaponInfoComponent* wep, f32 dt)
{
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
	return false;
}

void weaponFiringSystem(flecs::iter it, WeaponInfoComponent* wic, IrrlichtComponent* irrC)
{
	for (auto i : it) 
	{
		auto entityId = it.entity(i);
		auto wepInfo = &wic[i];
		if (wepInfo->type == WEP_NONE) continue;
		auto irrComp = &irrC[i];

		handleSpecialWepFunctions(entityId, it.delta_time());

		if (wepInfo->usesAmmunition)
		{
			if (wepInfo->clip == 0)
			{
				wepInfo->timeReloading += it.delta_time();
			}
		}

		if (wepInfo->isFiring && (wepInfo->timeSinceLastShot > wepInfo->firingSpeed))
		{
			if (wepInfo->usesAmmunition) ammoFire(it.entity(i), wepInfo, it.delta_time());
			else {
				createProjectileEntity(wepInfo->spawnPosition, wepInfo->firingDirection, entityId);
			}
			wepInfo->timeSinceLastShot = 0;
		}
		wepInfo->timeSinceLastShot += it.delta_system_time();
		if (wepInfo->timeSinceLastShot > wepInfo->firingSpeed)
		{
			// what is this?
			//manual check to keep it from spiralling off into infinity
			wepInfo->timeSinceLastShot = wepInfo->firingSpeed + .005f;
		}
	}
}
