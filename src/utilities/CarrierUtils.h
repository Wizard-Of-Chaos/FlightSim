#pragma once
#ifndef CARRIERUTILS_H
#define CARRIERUTILS_H
#include "BaseHeader.h"
#include "CarrierComponent.h"

/*
* Initializes a carrier with some of the parameters.
* Note that this does not set the ships involved; that should be done dynamically with further functions.
*/
void initializeCarrier(flecs::entity id, f32 spawnRate, u32 reserveShips, vector3df scale);

flecs::entity createCarrierFromId(u32 id, vector3df pos, vector3df rot);

flecs::entity createHumanCarrier(u32 carrId, vector3df pos, vector3df rot);
flecs::entity createAlienCarrier(u32 carrId, vector3df pos, vector3df rot);

#endif