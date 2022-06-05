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
	virtual void updateElement(flecs::entity playerId);
	IGUIImage* velocity;
	IGUIStaticText* velocityNum;
	IGUIStaticText* position;
	IGUIStaticText* rotationSpeed;
	IGUIStaticText* overrideStatus;
};

#endif 