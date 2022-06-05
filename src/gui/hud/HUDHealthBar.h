#pragma once
#ifndef HUDHEALTHBAR_H
#define HUDHEALTHBAR_H
#include "BaseHeader.h"
#include "HUDElement.h"
#include "ShipComponent.h"

/*
* The health bar element holds the data for the health bar, as well as some text to show the actual
* current health number. The health is adjusted in accordance with the % of the player's current health,
* containing logic to show the appropriate percentage of the actual health bar itself.
*/
class HUDResources : public HUDElement
{
public:
	HUDResources(IGUIElement* root, flecs::entity id);
	~HUDResources() override;
	virtual void updateElement(flecs::entity playerId);
	IGUIImage* health;
	IGUIImage* shield;
	IGUIImage* fuel;
	IGUIStaticText* fuelNum;
	IGUIStaticText* healthNum;
	IGUIStaticText* shieldNum;
	IGUIStaticText* ammoNums[MAX_HARDPOINTS];
};

#endif