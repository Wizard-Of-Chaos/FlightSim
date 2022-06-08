#pragma once
#ifndef HUDCONTACT_H
#define HUDCONTACT_H
#include "BaseHeader.h"
#include "HUDElement.h"

/*
* This element shows the given contact that's currently in range of the ship's sensors. It contains the contact,
* info to display itself, and logic to show when the contact is on-screen and a marker at the edge of the screen when
* it's not.
*/
class HUDContact : public HUDElement
{
public:
	HUDContact(IGUIElement* root, flecs::entity contactId, flecs::entity playerId);
	~HUDContact() override;
	virtual void updateElement(flecs::entity playerId);
	flecs::entity_t contact;

	IGUIImage* contactView;
	IGUIElement* offscreenMarker;
};

#endif 