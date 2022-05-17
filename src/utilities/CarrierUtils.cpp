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
	carr->spawnTimer = 0;
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

EntityId createHumanCarrier(u32 carrId, vector3df pos, vector3df rot)
{
	auto carrier = createCarrierFromId(carrId, pos, rot);
	auto ship = sceneManager->scene.get<ShipComponent>(carrier);
	CarrierData* carr = stateController->carrierData[ship->shipDataId];
	auto irr = sceneManager->scene.get<IrrlichtComponent>(carrier);
	irr->name = carr->name;
	initializeShipCollisionBody(carrier, carrId, true);
	initializeHealth(carrier, carr->health);
	initializeCarrier(carrier, carr->carrierComponent.spawnRate, carr->carrierComponent.reserveShips, carr->carrierComponent.scale);

	ShipInstance inst = newShipInstance();
	for (u32 i = 0; i < inst.ship.hardpointCount; ++i) {
		inst.weps[i] = stateController->weaponData[3]->weaponComponent;
	}

	auto carrcmp = sceneManager->scene.get<CarrierComponent>(carrier);
	carrcmp->shipTypeCount = 1;
	carrcmp->spawnShipTypes[0] = inst;

	initializePlayerFaction(carrier);
	initializeDefaultSensors(carrier);
	initializeDefaultAI(carrier);
	return carrier;
}

EntityId createAlienCarrier(u32 carrId, vector3df pos, vector3df rot)
{
	auto carrier = createCarrierFromId(carrId, pos, rot);
	auto ship = sceneManager->scene.get<ShipComponent>(carrier);
	CarrierData* carr = stateController->carrierData[ship->shipDataId];
	auto irr = sceneManager->scene.get<IrrlichtComponent>(carrier);
	irr->name = carr->name;

	initializeShipCollisionBody(carrier, carrId, true);
	initializeHealth(carrier, carr->health);
	initializeCarrier(carrier, carr->carrierComponent.spawnRate, carr->carrierComponent.reserveShips, carr->carrierComponent.scale);

	ShipInstance inst = newShipInstance();
	inst.ship = stateController->shipData[1]->shipComponent;
	for (u32 i = 0; i < inst.ship.hardpointCount; ++i) {
		inst.weps[i] = stateController->weaponData[1]->weaponComponent;
	}

	auto carrcmp = sceneManager->scene.get<CarrierComponent>(carrier);
	carrcmp->shipTypeCount = 1;
	carrcmp->spawnShipTypes[0] = inst;

	initializeHostileFaction(carrier);
	initializeDefaultSensors(carrier);
	initializeDefaultAI(carrier);
	return carrier;
}