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

		carrierCmp->spawnTimer += it.delta_system_time();
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