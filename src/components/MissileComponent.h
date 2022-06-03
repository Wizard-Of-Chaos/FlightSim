#pragma once
#ifndef MISSILECOMPONENT_H
#define MISSILECOMPONENT_H
#include "BaseHeader.h"

/*
* The missile info component is used alongside the WeaponInfoComponent to track data about missile weapons. Note that it's
* ALONGSIDE OF; this is just the extra data required for missiles to properly track. The two variables included are the rotational
* speed of the missile (e.g., how fast it can turn to track a target) and how fast the missile is allowed to go.
* It also includes how long it takes to lock onto a given ship.
*/
struct MissileInfoComponent
{
	f32 timeToLock; 
	f32 missileRotThrust;
	f32 maxMissileVelocity;
	IMesh* missileMesh;
	ITexture* missileTexture;
};

/*
* The missile projectile component is added onto a given missile projectile; it includes the same variables as the MissileInfoComponent
* and also the target that it's trying to move towards.
*/
struct MissileProjectileComponent
{
	flecs::entity target;
	//effectively a stripped-down ship component here
	f32 rotThrust;
	f32 maxVelocity;
};

#endif 