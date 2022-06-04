#pragma once
#ifndef DEFAULTAIUPDATESYSTEM_H
#define DEFAULTAIUPDATESYSTEM_H
#include "BaseHeader.h"
#include "AIComponent.h"
#include "SensorComponent.h"
#include "DefaultAIBehaviors.h"

/*
* Includes the default state check for the AI and the update system for the AI.
*/
//Checks as often as the AI component's reaction speed will allow it to. Updates state based on its environment.
void defaultAIStateCheck(flecs::entity id);
//The default update system for an AI component.
void defaultAIUpdateSystem(flecs::entity id, f32 dt);
#endif 