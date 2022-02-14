#pragma once

#ifndef PLAYERCOMPONENT_H
#define PLAYERCOMPONENT_H

#include "BaseHeader.h"

#include <vector>

class HUDElement;

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
};

#endif