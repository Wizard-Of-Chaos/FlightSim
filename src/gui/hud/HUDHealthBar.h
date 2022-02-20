#pragma once
#ifndef HUDHEALTHBAR_H
#define HUDHEALTHBAR_H
#include "BaseHeader.h"
#include "HUDElement.h"

class HUDHealthBar : public HUDElement
{
public:
	HUDHealthBar(SceneManager* manager, IGUIElement* root);
	~HUDHealthBar() override;
	virtual void updateElement(SceneManager* manager, EntityId playerId);
	IGUIImage* health;
	IGUIStaticText* healthNum;
};

#endif