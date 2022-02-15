#pragma once

#ifndef PLAYERCOMPONENT_H
#define PLAYERCOMPONENT_H

#include "BaseHeader.h"
#include "ECS.h"

#include <vector>
#include <map>

class HUDElement;
class HUDContact;

struct PlayerComponent
{
	ICameraSceneNode* camera;
	ISceneNode* target;
	//values for how much the camera swings around
	f32 slerpFactor = .009f;
	f32 velocityFactor = .1f;

	//hud
	IGUIElement* rootHUD;
	std::vector<HUDElement*> HUD;
	ISceneNode* activeSelection;
	std::map<EntityId, HUDContact*> trackedContacts;

	void removeContact(HUDContact* contact);
	void removeHUDElement(HUDElement* element);
};

#endif