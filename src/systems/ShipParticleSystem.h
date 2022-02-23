#pragma once
#ifndef SHIPPARTICLESYSTEM_H
#define SHIPPARTICLESYSTEM_H
#include "BaseHeader.h"
#include "ShipComponent.h"
#include "IrrlichtComponent.h"
#include "IrrlichtUtils.h"

void turnOnJets(IParticleEmitter* jet1, IParticleEmitter* jet2);
void turnOffJets(IParticleEmitter* jet1, IParticleEmitter* jet2);
void shipParticleSystem(Scene& scene, f32 dt);

#endif 