#pragma once
#ifndef DEFAULTAIUPDATESYSTEM_H
#define DEFAULTAIUPDATESYSTEM_H
#include "BaseHeader.h"
#include "ECS.h"
#include "AIComponent.h"
#include "DefaultAIBehaviors.h"

class SceneManager;
void updateContacts(SceneManager* manager, EntityId id);
void defaultAIStateCheck(SceneManager* manager, EntityId id);
void defaultAIUpdateSystem(SceneManager* manager, EntityId id, f32 dt);
#endif 