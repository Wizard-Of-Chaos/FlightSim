#pragma once
#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H
#include "BaseHeader.h"
#include "DamageTrackingComponent.h"
#include "HealthComponent.h"

//The damage system applies damage from all damage instances to their given entities.
void damageSystem(flecs::iter it, DamageTrackingComponent* dtc, HealthComponent* hc);

#endif 