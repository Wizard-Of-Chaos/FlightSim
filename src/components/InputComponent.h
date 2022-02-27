#pragma once

#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include "BaseHeader.h"

/*
* The input component takes in player input as part of the OnEvent call. Anything with an InputComponent
* attached gets updated in the ShipControlSystem. It tracks which keys are down, whether or not mouse control is on,
* the position (in pixels) of the mouse, the normalized position of the mouse, and whether or not right/left mouse are down.
* 
* TODO: Later, this should be abstracted out so that input gets translated to player-mapped keybinds for ship movement.
*/
struct InputComponent {
	bool keysDown[KEY_KEY_CODES_COUNT];
	bool mouseControlEnabled = false;
	bool safetyOverride = false;
	vector2df mousePosition;
	position2di mousePixPosition;
	bool leftMouseDown;
	bool rightMouseDown;
	bool isKeyDown(EKEY_CODE key) { return keysDown[key]; }
};

#endif