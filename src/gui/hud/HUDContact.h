#pragma once
#ifndef HUDCONTACT_H
#define HUDCONTACT_H
#include "BaseHeader.h"
#include "HUDElement.h"

class HUDContact : public HUDElement
{
public:
	HUDContact(IGUIElement* elem) : HUDElement(elem) {}
	virtual void updateElement(SceneManager* manager, PlayerComponent* player, ISceneNode* playerShip, InputComponent* input);
	EntityId contact;
	IGUIElement* marker;
};

#endif 