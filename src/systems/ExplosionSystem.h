#pragma once
#ifndef EXPLOSIONSYSTEM_H
#define EXPLOSIONSYSTEM_H
#include "ExplosionComponent.h"
#include "BaseHeader.h"

class SceneManager;

/*
* Handles explosions. Anything with an explosion component has its particle emissions decreased over the span of its lifetime.
*/
void explosionSystem(SceneManager* manager, f32 dt);

#endif 