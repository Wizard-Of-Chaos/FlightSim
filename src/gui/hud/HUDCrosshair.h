#pragma once
#ifndef HUDCROSSHAIR_H
#define HUDCROSSHAIR_H
#include "HUDElement.h"
#include "BaseHeader.h"

/*
* The crosshair element shows at a fixed distance to the player's "front". It updates when the player is turning around the place and
* displays the 3D point at that fixed distance. It contains info to display itself and the logic to update.
*/
class HUDCrosshair : public HUDElement
{
public:
	HUDCrosshair(SceneManager* man, IGUIElement* root);
	~HUDCrosshair() override;
	virtual void updateElement(SceneManager* manager, EntityId playerId);
	IGUIImage* crosshair;
};

#endif 