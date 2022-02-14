#pragma once

#ifndef HUDELEMENT_H
#define HUDELEMENT_H
#include "BaseHeader.h"
#include "PlayerComponent.h"
#include "InputComponent.h"
#include "ECS.h"

class SceneManager;

class HUDElement
{
public:
	HUDElement(IGUIElement* elem) : elem(elem) {}
	virtual void updateElement(SceneManager* manager, PlayerComponent* player, ISceneNode* playerShip, InputComponent* input) = 0;
protected:
	IGUIElement* elem;
};

#endif