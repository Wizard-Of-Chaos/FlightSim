#pragma once
#ifndef HUDCONTACT_H
#define HUDCONTACT_H
#include "BaseHeader.h"
#include "HUDElement.h"

class HUDContact : public HUDElement
{
public:
	HUDContact(SceneManager* man, EntityId id);
	~HUDContact() override;
	virtual void updateElement(SceneManager* manager, PlayerComponent* player, ISceneNode* playerShip, InputComponent* input);
	EntityId contact;

	IGUIImage* contactView;
	IGUIElement* offscreenMarker;
};

#endif 