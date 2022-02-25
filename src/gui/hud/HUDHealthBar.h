#pragma once
#ifndef HUDHEALTHBAR_H
#define HUDHEALTHBAR_H
#include "BaseHeader.h"
#include "HUDElement.h"

/*
* The health bar element holds the data for the health bar, as well as some text to show the actual
* current health number. The health is adjusted in accordance with the % of the player's current health,
* containing logic to show the appropriate percentage of the actual health bar itself.
*/
class HUDHealthBar : public HUDElement
{
public:
	HUDHealthBar(SceneManager* manager, IGUIElement* root);
	~HUDHealthBar() override;
	virtual void updateElement(SceneManager* manager, EntityId playerId);
	IGUIImage* health;
	IGUIStaticText* healthNum;
};

#endif