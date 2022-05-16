#include "CarrierUpdateSystem.h"
#include "GameController.h"
#include "GameStateController.h"
#include "GuiController.h"
#include "CarrierUtils.h"

void carrierUpdateSystem(f32 dt)
{
	for (auto id : SceneView<CarrierComponent, ShipComponent, FactionComponent, IrrlichtComponent>(sceneManager->scene)) {
		auto carr = sceneManager->scene.get<CarrierComponent>(id);
		auto irr = sceneManager->scene.get<IrrlichtComponent>(id);
		auto fac = sceneManager->scene.get<FactionComponent>(id);
		carr->spawnTimer += dt;

		if (carr->spawnTimer >= carr->spawnRate && carr->reserveShips > 0) {
			ShipInstance inst = carr->spawnShipTypes[0];
			vector3df spawnPos = getNodeDown(irr->node) * 15.f * carr->scale.Y;
			vector3df spawnRot = irr->node->getRotation();
			carrierSpawnShip(inst, spawnPos, spawnRot, fac);
			--carr->reserveShips;
			carr->spawnTimer = 0;
		}
	}
}