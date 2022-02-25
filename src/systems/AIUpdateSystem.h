#pragma once
#ifndef AIUPDATESYSTEM_H
#define AIUPDATESYSTEM_H
#include "BaseHeader.h"
#include "ECS.h"
#include "AIComponent.h"
#include "DefaultAIUpdateSystem.h"

class SceneManager;

//Updates all AI components in the scene (allied, friendly, and neutral ships).
void AIUpdateSystem(SceneManager* manager, f32 dt);

#endif 