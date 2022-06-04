#include "CarrierUpdateSystem.h"
#include "GameController.h"
#include "GameStateController.h"
#include "GuiController.h"
#include "CarrierUtils.h"

void carrierUpdateSystem(flecs::iter it, CarrierComponent* carr, IrrlichtComponent* irr, FactionComponent* fac)
{
	for (auto i : it) {
		auto carrierCmp = &carr[i];
		auto irrCmp = &irr[i];
		auto facCmp = &fac[i];

		carrierCmp->spawnTimer += it.delta_time();
		if (carrierCmp->spawnTimer >= carrierCmp->spawnRate && carrierCmp->reserveShips > 0) {
			ShipInstance inst = carrierCmp->spawnShipTypes[0];
			vector3df spawnPos = irrCmp->node->getAbsolutePosition() + getNodeDown(irrCmp->node) * 5.f * carrierCmp->scale.Y;
			vector3df spawnRot = irrCmp->node->getRotation();
			carrierSpawnShip(inst, spawnPos, spawnRot, facCmp);
			--carrierCmp->reserveShips;
			carrierCmp->spawnTimer = 0;
		}
	}
}

/*
void carrierUpdateSystem(f32 dt)
{
	for (auto id : SceneView<CarrierComponent, ShipComponent, FactionComponent, IrrlichtComponent>(sceneManager->scene)) 
	{
		auto carr = sceneManager->scene.get<CarrierComponent>(id);
		auto irr = sceneManager->scene.get<IrrlichtComponent>(id);
		auto fac = sceneManager->scene.get<FactionComponent>(id);
		carr->spawnTimer += dt;

		if (carr->spawnTimer >= carr->spawnRate && carr->reserveShips > 0) {
			ShipInstance inst = carr->spawnShipTypes[0];
			vector3df spawnPos = irr->node->getAbsolutePosition() + getNodeDown(irr->node) * 5.f * carr->scale.Y;
			vector3df spawnRot = irr->node->getRotation();
			carrierSpawnShip(inst, spawnPos, spawnRot, fac);
			--carr->reserveShips;
			carr->spawnTimer = 0;
		}
	}
}
*/