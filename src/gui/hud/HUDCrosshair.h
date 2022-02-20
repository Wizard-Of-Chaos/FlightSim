#pragma once
#ifndef HUDCROSSHAIR_H
#define HUDCROSSHAIR_H
#include "HUDElement.h"
#include "BaseHeader.h"

class HUDCrosshair : public HUDElement
{
public:
	HUDCrosshair(SceneManager* man, IGUIElement* root);
	~HUDCrosshair() override;
	virtual void updateElement(SceneManager* manager, EntityId playerId);
	IGUIImage* crosshair;
};

#endif 