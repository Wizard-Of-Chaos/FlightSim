#pragma once
#ifndef HEALTHANDSHIELDSYSTEMS_H
#define HEALTHANDSHIELDSYSTEMS_H
#include "BaseHeader.h"
#include "ECS.h"
#include "IrrlichtComponent.h"
#include "BulletRigidBodyComponent.h"
#include "HealthComponent.h"
#include "GameFunctions.h"
//Updates health for all entities in the scene. If health is 0, removes it from the scene.
void healthSystem();

//Updates shield values for all entities in the scene.
void shieldSystem(f32 dt);

#endif 

