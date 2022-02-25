#pragma once

#ifndef DEFAULTAIBEHAVIORS_H
#define DEFAULTAIBEHAVIORS_H
#include "BaseHeader.h"
#include "AIComponent.h"
#include "ECS.h"

class SceneManager;

/*
* Holds the different behaviors for the default AI. Called on AIComponents as part of their state.
*/

//The default "idle" behavior for an AI. Causes the AI to stop what it's doing and stay still.
void defaultIdleBehavior(SceneManager* manager, EntityId id, f32 dt);
//The default "flee" behavior for an AI. Turns away from whatever's shooting at it and accelerates as fast as it can.
void defaultFleeBehavior(SceneManager* manager, EntityId id, EntityId fleeTarget, f32 dt);
//The default "pursuit" behavior for an AI. Attempts to get behind the target and then stay there. If the target is
//within a certain cone, it opens fire.
void defaultPursuitBehavior(SceneManager* manager, EntityId id, EntityId pursuitTarget, f32 dt);
#endif 