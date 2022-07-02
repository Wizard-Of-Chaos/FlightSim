#pragma once
#ifndef EXPLOSIONCOMPONENT_H
#define EXPLOSIONCOMPONENT_H
#include "BaseHeader.h"
/*
* The explosion component is exactly what it says on the tin: it's a way to create explosions.
* The duration is how long the explosion lasts. The lifetime is how long the explosion has currently lasted.
* When time's up, it removes itself from the scene.
*/
struct ExplosionComponent
{
	IParticleSystemSceneNode* explosion;
	ILightSceneNode* light;
	f32 duration;
	bool hasExploded;
	f32 lifetime;
	f32 force;
	f32 radius;
	f32 damage;
};

#endif 