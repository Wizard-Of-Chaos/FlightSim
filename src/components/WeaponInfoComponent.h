#pragma once

#ifndef WEAPONINFOCOMPONENT_H
#define WEAPONINFOCOMPONENT_H
#include "BaseHeader.h"
#include "DamageTrackingComponent.h"
#include <map>

/*
* This enum holds the different types of weapon that a gun can be.
* The PHYS tag means its a goofy weapon.
*/

enum WEAPON_TYPE {
	WEP_NONE = -1,
	WEP_PLASMA = 0,
	WEP_MISSILE = 1,
	WEP_GRAPPLE = 2,
	WEP_KINETIC = 3,
	WEP_PHYS_IMPULSE = 4,
	WEP_PHYS_BOLAS = 5,
	WEP_LASER = 6
};

//A map for convenience when pulling values out of files.
const std::map<std::string, WEAPON_TYPE> weaponStrings{
	{"none", WEP_NONE},
	{"plasma", WEP_PLASMA},
	{"missile", WEP_MISSILE},
	{"grapple", WEP_GRAPPLE},
	{"kinetic", WEP_KINETIC},
	{"laser", WEP_LASER},
	{"phys_impulse", WEP_PHYS_IMPULSE},
	{"phys_bolas", WEP_PHYS_BOLAS}
};

//WEAPON ENTITIES:
//Irrlicht component, weapon info component

/*
* The weapon info component holds some basic information about the weapon. It holds
* the projectile speed, firing speed, range, damage, and some information to determine when to fire the gun.
* It also holds the type of weapon, which is loaded in AttributeLoaders as an integer.
* 
* In order to load a weapon, first the data is pulled from the appropriate .gdat file into the stateController. After that, the weapon
* is loaded onto the given ship and the textures / model / projectiles are loaded. When fired from the weaponFiringSystem function, a projectile entity
* gets created and added to the scene, and it eventually gets despawned when it hits something or goes out of range. In order to add a new weapon, you'll
* need to add your new attributes file where it defines the basic information and texture/model locations, then any additional behaviors you might want depending
* on the weapon type (i.e., you implemented a new type and need to define how the hell a teapot launcher works) and finally if you've added extra data (or extra components) you'll need
* to add in some extra loading information to the relevant functions in AttributeReaders.cpp (loadWeaponData and loadWeapon).
*/
struct WeaponInfoComponent
{
	u32 wepDataId;
	WEAPON_TYPE type;
	DAMAGE_TYPE dmgtype;
	f32 firingSpeed; //how long it should take in seconds between shots
	f32 projectileSpeed; //how fast the projectile goes
	f32 lifetime; //how long the projectile lasts in the world
	f32 timeSinceLastShot;
	f32 damage;
	bool isFiring;
	vector3df firingDirection;
	vector3df spawnPosition;
	bool usesAmmunition;
	u32 maxAmmunition;
	u32 ammunition;
	u32 clip;
	u32 maxClip;
	f32 timeReloading;
	f32 reloadTime;

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
	f32 lifetime;
	f32 currentLifetime;
	vector3df startPos;
};

#endif