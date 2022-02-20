#pragma once
#ifndef HUDCONTACT_H
#define HUDCONTACT_H
#include "BaseHeader.h"
#include "HUDElement.h"

class HUDContact : public HUDElement
{
public:
	HUDContact(SceneManager* man, IGUIElement* root, EntityId contactId);
	~HUDContact() override;
	virtual void updateElement(SceneManager* manager, EntityId playerId);
	EntityId contact;

	IGUIImage* contactView;
	IGUIElement* offscreenMarker;
};

#endif 