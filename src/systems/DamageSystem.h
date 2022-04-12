#pragma once
#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H
#include "BaseHeader.h"
#include "ECS.h"
#include "DamageTrackingComponent.h"

class SceneManager;
class GameController;

void damageSystem(SceneManager* manager, f32 dt);

#endif 