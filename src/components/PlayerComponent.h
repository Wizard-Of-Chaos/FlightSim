#pragma once

#ifndef PLAYERCOMPONENT_H
#define PLAYERCOMPONENT_H

#include "BaseHeader.h"

struct PlayerComponent
{
	ICameraSceneNode* camera;
	ISceneNode* target;
	//values for how much the camera swings around
	f32 slerpFactor = .009f;
	f32 velocityFactor = .1f;

	ISceneNode* activeSelection;
	IGUIImage* crosshairGui;
	IGUIImage* selectionGui;
};

#endif