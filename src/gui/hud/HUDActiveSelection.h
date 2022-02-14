#pragma once
#ifndef HUDACTIVESELECTION_H
#define HUDACTIVESELECTION_H
#include "BaseHeader.h"
#include "HUDElement.h"

class HUDActiveSelection : public HUDElement
{
public:
	HUDActiveSelection(IGUIElement* elem) : HUDElement(elem) {}
	virtual void updateElement(SceneManager* manager, PlayerComponent* player, ISceneNode* playerShip, InputComponent* input);
};

#endif 
