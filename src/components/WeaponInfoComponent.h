#pragma once

#ifndef WEAPONINFOCOMPONENT_H
#define WEAPONINFOCOMPONENT_H
#include "BaseHeader.h"
#include "DamageTrackingComponent.h"
#include "ECS.h"

/*
* This enum holds the different types of weapons that a thing can be.
* Currently only WEP_PLASMA is properly implemented. More to come.
*/

enum WEAPON_TYPE {
	WEP_NONE = -1,
	WEP_PLASMA = 0,
	WEP_MISSILE = 1,
	WEP_GRAPPLE = 2,
	WEP_KINETIC = 3,
	WEP_PHYS_IMPULSE = 4
};

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
	DAMAGE_TYPE dmgtype;
	f32 firingSpeed; //how long it should take in seconds between shots
	f32 projectileSpeed; //how fast the projectile goes
	f32 range; //how far the projectile goes
	f32 timeSinceLastShot;
	f32 damage;
	bool isFiring;
	vector3df firingDirection;
	vector3df spawnPosition;
	ITexture* particle;
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
	DAMAGE_TYPE dmgtype;
	f32 damage;
	f32 speed;
	f32 range;
	vector3df startPos;
};

#endif