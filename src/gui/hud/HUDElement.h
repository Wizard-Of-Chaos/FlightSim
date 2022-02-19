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
	HUDElement(SceneManager* man) {}
	virtual ~HUDElement() {}
	virtual void updateElement(SceneManager* manager, PlayerComponent* player, ISceneNode* playerShip, InputComponent* input) = 0;
};

#endif