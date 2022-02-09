#pragma once

#ifndef DEFAULTAIBEHAVIORS_H
#define DEFAULTAIBEHAVIORS_H
#include "BaseHeader.h"
#include "AIComponent.h"
#include "ECS.h"

class SceneManager;

void defaultIdleBehavior(SceneManager* manager, EntityId id, f32 dt);
void defaultFleeBehavior(SceneManager* manager, EntityId id, EntityId fleeTarget, f32 dt);
void defaultPursuitBehavior(SceneManager* manager, EntityId id, EntityId pursuitTarget, f32 dt);
#endif 