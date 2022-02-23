#pragma once
#ifndef SHIPUPDATESYSTEM_H
#define SHIPUPDATESYSTEM_H
#include "BaseHeader.h"
#include "ShipComponent.h"
#include "IrrlichtComponent.h"
#include "BulletRigidBodyComponent.h"
#include "IrrlichtUtils.h"
#include "ShipMovementUtils.h"

void turnOnJets(IParticleEmitter* jet1, IParticleEmitter* jet2);
void turnOffJets(IParticleEmitter* jet1, IParticleEmitter* jet2);
void shipUpdateSystem(Scene& scene, f32 dt);

#endif 