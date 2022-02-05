#pragma once

#ifndef SHIPMOVEMENTSYSTEM_H
#define SHIPMOVEMENTSYSTEM_H
#include "BaseHeader.h"

#include "ECS.h"
#include "InputComponent.h"
#include "IrrlichtComponent.h"
#include "BulletRigidBodyComponent.h"
#include "ShipMovementUtils.h"
#include "ShipComponent.h"

void shipMovementSystem(Scene& scene, f32 dt);

#endif