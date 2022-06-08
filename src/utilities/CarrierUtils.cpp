#include "CarrierUtils.h"
#include "GameController.h"
#include "GameStateController.h"
#include "GuiController.h"

void initializeCarrier(flecs::entity id, f32 spawnRate, u32 reserveShips, vector3df scale)
{
	auto carr = id.get_mut<CarrierComponent>();
	carr->reserveShips = reserveShips;
	carr->spawnRate = spawnRate;
	carr->scale = scale;
	carr->spawnTimer = 0;
}

flecs::entity createCarrierFromId(u32 id, vector3df pos, vector3df rot)
{
	flecs::entity carrier = game_world->entity();
	loadShip(id, carrier, true);
	auto ship = carrier.get_mut<ShipComponent>();
	auto irr = carrier.get_mut<IrrlichtComponent>();
	irr->node->setPosition(pos);
	irr->node->setRotation(rot);
	for (u32 i = 0; i < MAX_HARDPOINTS; ++i) {
		ship->weapons[i] = INVALID_ENTITY;
	}
	initializeShipParticles(carrier);
	return carrier;
}

flecs::entity createHumanCarrier(u32 carrId, vector3df pos, vector3df rot)
{
	auto carrier = createCarrierFromId(carrId, pos, rot);
	auto ship = carrier.get_mut<ShipComponent>();
	CarrierData* carr = stateController->carrierData[ship->shipDataId];
	auto irr = carrier.get_mut<IrrlichtComponent>();
	irr->name = carr->name;
	initializeShipCollisionBody(carrier, carrId, true);
	initializeHealth(carrier, carr->health);
	initializeCarrier(carrier, carr->carrierComponent.spawnRate, carr->carrierComponent.reserveShips, carr->carrierComponent.scale);

	ShipInstance inst = newShipInstance();
	for (u32 i = 0; i < inst.ship.hardpointCount; ++i) {
		inst.weps[i] = stateController->weaponData[3]->weaponComponent;
	}

	auto carrcmp = carrier.get_mut<CarrierComponent>();
	carrcmp->shipTypeCount = 1;
	carrcmp->spawnShipTypes[0] = inst;

	initializePlayerFaction(carrier);
	initializeDefaultSensors(carrier);
	initializeDefaultAI(carrier);
	return carrier;
}

flecs::entity createAlienCarrier(u32 carrId, vector3df pos, vector3df rot)
{
	auto carrier = createCarrierFromId(carrId, pos, rot);
	auto ship = carrier.get_mut<ShipComponent>();
	CarrierData* carr = stateController->carrierData[ship->shipDataId];
	auto irr = carrier.get_mut<IrrlichtComponent>();
	irr->name = carr->name;

	initializeShipCollisionBody(carrier, carrId, true);
	initializeHealth(carrier, carr->health);
	initializeCarrier(carrier, carr->carrierComponent.spawnRate, carr->carrierComponent.reserveShips, carr->carrierComponent.scale);

	ShipInstance inst = newShipInstance();
	inst.ship = stateController->shipData[1]->shipComponent;
	for (u32 i = 0; i < inst.ship.hardpointCount; ++i) {
		inst.weps[i] = stateController->weaponData[1]->weaponComponent;
	}

	auto carrcmp = carrier.get_mut<CarrierComponent>();
	carrcmp->shipTypeCount = 1;
	carrcmp->spawnShipTypes[0] = inst;

	initializeHostileFaction(carrier);
	initializeDefaultSensors(carrier);
	initializeDefaultAI(carrier);
	return carrier;
}