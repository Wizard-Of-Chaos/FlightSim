#pragma once
#ifndef SHIPUPDATESYSTEM_H
#define SHIPUPDATESYSTEM_H
#include "BaseHeader.h"
#include "ShipComponent.h"
#include "IrrlichtComponent.h"
#include "BulletRigidBodyComponent.h"
#include "IrrlichtUtils.h"
#include "ShipMovementUtils.h"

//Turns on the particle emitter.
void jetOn(IParticleEmitter* jet);
//Turns off the particle emitter.
void jetOff(IParticleEmitter* jet);
//Sets two particle emitters on at once - useful for rotations.
void jetPairOn(IParticleEmitter* jet1, IParticleEmitter* jet2);
//Turns off two particle emitters at once.
void jetPairOff(IParticleEmitter* jet1, IParticleEmitter* jet2);
//Sets the direction of a pair of emitters.
void setPairDir(IParticleEmitter* jet1, IParticleEmitter* jet2, vector3df dir);

/*
* Actually updates the ship component in a given scene. This function plays the audio associated with each movement,
* updates the particle effects while moving (with the jet functions above), and applies the torques and forces from
* movement. This function is used both by the player and the AI - they all follow the same set of rules with pitch, yaw,
* torque, all of that.
*/
void shipUpdateSystem(Scene& scene, f32 dt);

#endif 