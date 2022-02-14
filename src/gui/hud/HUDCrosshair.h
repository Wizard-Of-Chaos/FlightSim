#pragma once
#ifndef HUDCROSSHAIR_H
#define HUDCROSSHAIR_H
#include "HUDElement.h"
#include "BaseHeader.h"

class HUDCrosshair : public HUDElement
{
public:
	HUDCrosshair(IGUIElement* elem) : HUDElement(elem) {}
	virtual void updateElement(SceneManager* manager, PlayerComponent* player, ISceneNode* playerShip, InputComponent* input);
};

#endif 