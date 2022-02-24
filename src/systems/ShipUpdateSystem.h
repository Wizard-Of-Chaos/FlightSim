#pragma once
#ifndef SHIPUPDATESYSTEM_H
#define SHIPUPDATESYSTEM_H
#include "BaseHeader.h"
#include "ShipComponent.h"
#include "IrrlichtComponent.h"
#include "BulletRigidBodyComponent.h"
#include "IrrlichtUtils.h"
#include "ShipMovementUtils.h"

void jetOn(IParticleEmitter* jet);
void jetOff(IParticleEmitter* jet);
void jetPairOn(IParticleEmitter* jet1, IParticleEmitter* jet2);
void jetPairOff(IParticleEmitter* jet1, IParticleEmitter* jet2);
void setPairDir(IParticleEmitter* jet1, IParticleEmitter* jet2, vector3df dir);
void shipUpdateSystem(Scene& scene, f32 dt);

#endif 