#pragma once

#ifndef WEAPONINFOCOMPONENT_H
#define WEAPONINFOCOMPONENT_H
#include "BaseHeader.h"
#include "ECS.h"

enum WEAPON_TYPE {
	WEP_PLASMA = 0,
	WEP_MISSILE = 1,
	WEP_GRAPPLE = 2
};

const f32 DEFAULT_FIRING_SPEED = .5f;
const f32 DEFAULT_PROJECTILE_SPEED = 20.f;
const f32 DEFAULT_WEAPON_RANGE = 300.f;
const WEAPON_TYPE DEFAULT_WEAPON_TYPE = WEP_PLASMA;
const f32 DEFAULT_WEAPON_DAMAGE = 15.f;

//WEAPON ENTITIES:
//Irrlicht component, weapon info component

struct WeaponInfoComponent
{
	WEAPON_TYPE type;
	f32 firingSpeed; //how long it should take in seconds between shots
	f32 projectileSpeed; //how fast the projectile goes
	f32 range; //how far the projectile goes
	f32 timeSinceLastShot;
	f32 damage;
	bool isFiring;
};

//PROJECTILE ENTITIES:
//Irrlicht component, projectile info component, rigid body component

struct ProjectileInfoComponent
{
	WEAPON_TYPE type;
	f32 damage;
	f32 speed;
	f32 range;
	vector3df startPos;
};

#endif