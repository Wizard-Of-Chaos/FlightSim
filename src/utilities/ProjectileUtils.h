#pragma once
#ifndef PROJECTILEUTILS_H
#define PROJECTILEUTILS_H
#include "BaseHeader.h"
#include "WeaponInfoComponent.h"
#include "IrrlichtComponent.h"
#include "BulletRigidBodyComponent.h"
#include "MissileComponent.h"
#include "ShipMovementUtils.h"

//Creates a projectile entity and shoots it in the indicated direction. Assigns information to the projectile (range, speed, etc)
//based on the weapon that shot it. Returns the ID of the projectile.
EntityId createProjectileEntity(vector3df spawnPos, vector3df direction, EntityId weaponId);
//Removes the given projectile from the scene.
void destroyProjectile(EntityId projectile);

//Adds on the info for a plasma projectile as well as particle effects.
void createPlasmaProjectile(EntityId projId, vector3df dir, vector3df spawn);

//Adds on the info for a missile projectile as well as particle effects.
void createMissileProjectile(EntityId projId, MissileInfoComponent* missInfo, vector3df dir, vector3df spawn);

void createKineticProjectile(EntityId projId, vector3df dir, vector3df spawn);

//Creates an explosion at the impact point of a projectile. Way smaller than the other one.
EntityId projectileImpact(vector3df position, f32 duration);

//Causes the missile to track its target and fly towards it.
void missileGoTo(EntityId id, f32 dt);
#endif 