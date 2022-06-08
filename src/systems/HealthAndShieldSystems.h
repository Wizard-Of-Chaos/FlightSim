#pragma once
#ifndef HEALTHANDSHIELDSYSTEMS_H
#define HEALTHANDSHIELDSYSTEMS_H
#include "BaseHeader.h"
#include "IrrlichtComponent.h"
#include "BulletRigidBodyComponent.h"
#include "HealthComponent.h"
#include "GameFunctions.h"
#include "ShieldComponent.h"
//Updates health for all entities in the scene. If health is 0, removes it from the scene.
void healthSystem(flecs::iter it, HealthComponent* hc);

//Updates shield values for all entities in the scene.
void shieldSystem(flecs::iter it, ShieldComponent* sc);

#endif 

