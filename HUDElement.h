#pragma once

#ifndef HUDELEMENT_H
#define HUDELEMENT_H
#include "BaseHeader.h"
#include "PlayerComponent.h"
#include "InputComponent.h"
#include "ECS.h"

class SceneManager;
class GameController;

struct HUDElement
{
	HUDElement(IGUIElement* newElem) : elem(newElem) {}
	//the chunk of gui in question
	IGUIElement* elem;
	//updates what the element does every tick
	virtual void updateElement(SceneManager* manager, PlayerComponent* player, ISceneNode* playerShip, InputComponent* input) = 0;
};

#endif