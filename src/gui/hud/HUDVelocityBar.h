#pragma once
#ifndef HUDVELOCITYBAR_H
#define HUDVELOCITYBAR_H
#include "BaseHeader.h"
#include "HUDElement.h"

class HUDVelocityBar : public HUDElement
{
public:
	HUDVelocityBar(IGUIElement* root);
	~HUDVelocityBar() override;
	virtual void updateElement(EntityId playerId);
	IGUIImage* velocity;
	IGUIStaticText* velocityNum;
	IGUIStaticText* position;
	IGUIStaticText* rotationSpeed;
	IGUIStaticText* overrideStatus;
};

#endif 