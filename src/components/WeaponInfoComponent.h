#pragma once

#ifndef WEAPONINFOCOMPONENT_H
#define WEAPONINFOCOMPONENT_H
#include "BaseHeader.h"
#include "ECS.h"

/*
* This enum holds the different types of weapons that a thing can be.
* Currently only WEP_PLASMA is properly implemented. More to come.
*/
enum WEAPON_TYPE {
	WEP_NONE = -1,
	WEP_PLASMA = 0,
	WEP_MISSILE = 1,
	WEP_GRAPPLE = 2
};

//Default weapon values.
const f32 DEFAULT_FIRING_SPEED = .5f;
const f32 DEFAULT_PROJECTILE_SPEED = 20.f;
const f32 DEFAULT_WEAPON_RANGE = 300.f;
const WEAPON_TYPE DEFAULT_WEAPON_TYPE = WEP_PLASMA;
const f32 DEFAULT_WEAPON_DAMAGE = 15.f;

//WEAPON ENTITIES:
//Irrlicht component, weapon info component

/*
* The weapon info component holds some basic information about the weapon. It holds
* the projectile speed, firing speed, range, damage, and some information to determine when to fire the gun.
* It also holds the type of weapon, which is loaded in AttributeLoaders as an integer.
* 
* In the future, it will also hold the texture for its own projectile.
*/
struct WeaponInfoComponent
{
	WEAPON_TYPE type;
	f32 firingSpeed; //how long it should take in seconds between shots
	f32 projectileSpeed; //how fast the projectile goes
	f32 range; //how far the projectile goes
	f32 timeSinceLastShot;
	f32 damage;
	bool isFiring;
	vector3df firingDirection;
	vector3df spawnPosition;
};

//PROJECTILE ENTITIES:
//Irrlicht component, projectile info component, rigid body component

/*
* The projectile info component duplicates the information from the weapon info component that it spawned from.
* It also holds its own start position and its range, as well as a bullet component to be able to collide with ships properly.
*/
struct ProjectileInfoComponent
{
	WEAPON_TYPE type;
	f32 damage;
	f32 speed;
	f32 range;
	vector3df startPos;
};

#endif