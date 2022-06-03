#pragma once

#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H
#include "BaseHeader.h"

//UNUSED AT THE MOMENT
//This enum will be used to track actual keybinds later.
enum INPUT {
	IN_THRUST_FORWARDS,
	IN_STRAFE_BACKWARDS,
	IN_STRAFE_LEFT,
	IN_STRAFE_RIGHT,
	IN_STRAFE_UP,
	IN_STRAFE_DOWN,
	IN_AFTERBURNER,
	IN_PITCH_UP,
	IN_PITCH_DOWN,
	IN_YAW_LEFT,
	IN_YAW_RIGHT,
	IN_ROLL_LEFT,
	IN_ROLL_RIGHT,
	IN_TOGGLE_SAFETY,
	IN_PAUSE_MENU,
	IN_TOGGLE_MOUSE,
	IN_TOGGLE_HUD,
	IN_TOGGLE_THROTTLE
};
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
	line3df cameraRay;
	bool leftMouseDown;
	bool rightMouseDown;
	bool isKeyDown(EKEY_CODE key) { return keysDown[key]; }
};

#endif