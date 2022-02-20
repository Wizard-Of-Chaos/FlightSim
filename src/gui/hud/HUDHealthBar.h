#pragma once
#ifndef HUDHEALTHBAR_H
#define HUDHEALTHBAR_H
#include "BaseHeader.h"
#include "HUDElement.h"

class HUDHealthBar : public HUDElement
{
public:
	HUDHealthBar(SceneManager* manager);
	~HUDHealthBar() override;
	virtual void updateElement(SceneManager* manager, PlayerComponent* player, ISceneNode* playerShip, InputComponent* input);
	IGUIImage* health;
};

#endif