#pragma once
#ifndef SHIPINSTANCE_H
#define SHIPINSTANCE_H
#include "BaseHeader.h"
#include "ShipComponent.h"
#include "WeaponInfoComponent.h"
#include "HealthComponent.h"

/*
* This "instance" effectively tracks a given instance of a ship - its fuel, its ammunition, its health, et cetera - so that it can persist between
* scenarios. It doubles as a loadout structure where you can quickly load weapons and ship information from a given loadout. This is used both on the campaign
* and by carriers.
*/
struct ShipInstance
{
	ShipComponent ship;
	HealthComponent hp;
	WeaponInfoComponent weps[MAX_HARDPOINTS]; //since weapon info components have their ids, we should be able to pull any necessary data back out when loading
	WeaponInfoComponent physWep;
};

#endif 