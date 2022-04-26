#pragma once
#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H
#include "BaseHeader.h"
#include "ECS.h"
#include "DamageTrackingComponent.h"

//The damage system applies damage from all damage instances to their given entities.
void damageSystem(f32 dt);

#endif 