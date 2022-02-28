#pragma once
#ifndef HUDVELOCITYBAR_H
#define HUDVELOCITYBAR_H
#include "BaseHeader.h"
#include "HUDElement.h"

class HUDVelocityBar : public HUDElement
{
public:
	HUDVelocityBar(SceneManager* manager, IGUIElement* root);
	~HUDVelocityBar() override;
	virtual void updateElement(SceneManager* manager, EntityId playerId);
	IGUIImage* velocity;
	IGUIStaticText* velocityNum;
	IGUIStaticText* position;
	IGUIStaticText* rotationSpeed;
	IGUIStaticText* overrideStatus;
};

#endif 