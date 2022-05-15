#include "CarrierUtils.h"
#include "GameController.h"
#include "GameStateController.h"
#include "SceneManager.h"
#include "GuiController.h"

void initializeCarrier(EntityId id, f32 spawnRate, u32 reserveShips, vector3df scale)
{
	auto carr = sceneManager->scene.assign<CarrierComponent>(id);
	carr->reserveShips = reserveShips;
	carr->spawnRate = spawnRate;
	carr->scale = scale;
}

EntityId createCarrierFromId(u32 id, vector3df pos, vector3df rot)
{
	EntityId carrier = sceneManager->scene.newEntity();
	loadShip(id, carrier, true);
	auto ship = sceneManager->scene.get<ShipComponent>(carrier);
	auto irr = sceneManager->scene.get<IrrlichtComponent>(carrier);
	irr->node->setPosition(pos);
	irr->node->setRotation(rot);
	for (u32 i = 0; i < MAX_HARDPOINTS; ++i) {
		ship->weapons[i] = INVALID_ENTITY;
	}
	initializeShipParticles(carrier);
	return carrier;
}

EntityId createDefaultHumanCarrier(vector3df pos, vector3df rot)
{
	u32 carrId = 0;
	auto carrier = createCarrierFromId(carrId, pos, rot);
	auto ship = sceneManager->scene.get<ShipComponent>(carrier);
	CarrierData* carr = stateController->carrierData[ship->shipDataId];
	auto irr = sceneManager->scene.get<IrrlichtComponent>(carrier);
	irr->name = "Chaos Theory";
	initializeNeutralFaction(carrier);
	initializeShipCollisionBody(carrier, carrId, true);
	initializeHealth(carrier, carr->health);
	return carrier;
}