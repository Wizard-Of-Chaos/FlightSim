#pragma once

#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include "BaseHeader.h"

struct InputComponent {
	bool keysDown[KEY_KEY_CODES_COUNT];
	bool mouseControlEnabled = false;
	vector2df mousePosition;
	position2di mousePixPosition;
	bool leftMouseDown;
	bool rightMouseDown;
	bool isKeyDown(EKEY_CODE key) { return keysDown[key]; }
};

#endif