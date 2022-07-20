#pragma once
#ifndef THRUSTSYSTEM_H
#define THRUSTSYSTEM_H
#include "BaseHeader.h"
#include "ThrustComponent.h"
#include "HealthComponent.h"
#include "BulletRigidBodyComponent.h"
#include "IrrlichtComponent.h"

void thrustSystem(flecs::iter it, ThrustComponent* thrc, HealthComponent* hpc, BulletRigidBodyComponent* rbcs, IrrlichtComponent* irrc);

#endif 