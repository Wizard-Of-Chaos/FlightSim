#pragma once
#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H
#include "BaseHeader.h"
#include "ECS.h"

//This system adjusts the position of all sound instances to where the thing causing them currently is.
void soundSystem(f32 dt);

#endif 