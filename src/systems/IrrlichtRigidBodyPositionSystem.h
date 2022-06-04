#pragma once

#ifndef IRRLICHTRIGIDBODYPOSITIONSYSTEM_H
#define IRRLICHTRIGIDBODYPOSITIONSYSTEM_H
#include "BaseHeader.h"
#include "IrrlichtComponent.h"
#include "BulletRigidBodyComponent.h"
#include "ShipMovementUtils.h"

//This function allows Irrlicht and Bullet to talk to each other. It checks all entities with Bullet components and Irrlicht components
//and makes it so that the Bullet calculations (for rotation, movement, and such) affect the Irrlicht component and makes the thing actually
//move.
void irrlichtRigidBodyPositionSystem(flecs::iter it, BulletRigidBodyComponent* rbcs, IrrlichtComponent* irrc);

#endif
