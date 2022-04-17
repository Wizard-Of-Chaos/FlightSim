#include "DamageSystem.h"
#include "GameController.h"
#include "SceneManager.h"
#include <iostream>

void handleInstance(DamageInstance& inst, HealthComponent* hp)
{
	hp->health -= inst.amount;
}

void handleShieldedInstance(DamageInstance& inst, HealthComponent* hp, ShieldComponent* shield)
{
	f32 overflow = 0;
	if (shield) {
		shield->shields -= inst.amount;
		if (shield->shields <= 0) {
			overflow += -shield->shields;
			shield->shields = 0;
		}
		shield->timeSinceLastHit = 0;
	}
	else {
		overflow = inst.amount;
	}
	if (overflow > 0) handleInstance(inst, hp);
}

void handleEMPInstance(DamageInstance& inst, ShieldComponent* shield)
{
	if (shield) {
		shield->shields -= inst.amount;
		shield->timeSinceLastHit = 0;
	}
}

void damageSystem(f32 dt)
{
	for (auto id : SceneView<DamageTrackingComponent, HealthComponent>(sceneManager->scene)) { //making the assumption that anything that can take damage has health
		auto dmg = sceneManager->scene.get<DamageTrackingComponent>(id);
		auto hp = sceneManager->scene.get<HealthComponent>(id);
		auto shld = sceneManager->scene.get<ShieldComponent>(id);
		auto irr = sceneManager->scene.get<IrrlichtComponent>(id);
		auto obst = sceneManager->scene.get<ObstacleComponent>(id);

		for (DamageInstance inst : dmg->instances) {
			switch (inst.type) {
			case DAMAGE_TYPE::NONE:
				break;
			case DAMAGE_TYPE::EXPLOSIVE:
				handleShieldedInstance(inst, hp, shld);
				break;
			case DAMAGE_TYPE::IMPACT:
				if (inst.time >= dmg->lastDamageTime + 500 && !obst) gameController->registerSoundInstance(id, sceneManager->defaults.crunch);
				handleInstance(inst, hp);
				break;
			case DAMAGE_TYPE::VELOCITY:
				handleInstance(inst, hp);
				break;
			case DAMAGE_TYPE::PLASMA:
				handleShieldedInstance(inst, hp, shld);
				break;
			case DAMAGE_TYPE::KINETIC:
				handleShieldedInstance(inst, hp, shld);
				break;
			case DAMAGE_TYPE::EMP:
				handleEMPInstance(inst, shld);
				break;
			default: //spit an error here
				break;
			}
			dmg->lastDamageType = inst.type;
			dmg->lastDamageTime = inst.time;
		}
		dmg->instances.clear();

	}
}