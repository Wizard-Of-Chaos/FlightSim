#pragma once
#ifndef SHIPPARTICLECOMPONENT_H
#define SHIPPARTICLECOMPONENT_H
#include "BaseHeader.h"

struct ShipParticleComponent
{
	//Positions on the ship for where the thrust emissions come from.
	IParticleSystemSceneNode* upJetEmit[2];
	IParticleSystemSceneNode* downJetEmit[2];
	IParticleSystemSceneNode* leftJetEmit[2];
	IParticleSystemSceneNode* rightJetEmit[2];
	IParticleSystemSceneNode* reverseJetEmit[2];
	IVolumeLightSceneNode* engineJetEmit;
	ILightSceneNode* engineLight; //The engine light is on! Check your oil.
};


#endif 