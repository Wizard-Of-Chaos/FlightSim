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
	HUDElement(SceneManager* man, IGUIElement* root) {}
	virtual ~HUDElement() {}
	virtual void updateElement(SceneManager* manager, EntityId playerId) = 0;
};

#endif