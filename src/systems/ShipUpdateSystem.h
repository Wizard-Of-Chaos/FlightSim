#pragma once
#ifndef SHIPUPDATESYSTEM_H
#define SHIPUPDATESYSTEM_H
#include "BaseHeader.h"
#include "ShipComponent.h"
#include "IrrlichtComponent.h"
#include "BulletRigidBodyComponent.h"
#include "ShipParticleComponent.h"
#include "IrrlichtUtils.h"
#include "ShipMovementUtils.h"

/*
* Actually updates the ship component in a given scene. This function plays the audio associated with each movement,
* updates the particle effects while moving (with the jet functions above), but DOES NOT apply torques and forces - that's 
* handled by the thrust system. Instead this system will adjust the values on thrust components if they do things like, say, go over
* the safety limits.
*/

void shipUpdateSystem(flecs::iter it, ThrustComponent* thrc, ShipComponent* shpc, BulletRigidBodyComponent* rbcs, IrrlichtComponent* irrc, ShipParticleComponent* prtc);

#endif 