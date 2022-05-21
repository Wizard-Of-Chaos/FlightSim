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

//Adds on the info for a kinetic projectile as well as particle effects.
void createKineticProjectile(EntityId projId, vector3df dir, vector3df spawn);

//Creates an explosion at the impact point of a projectile. Way smaller than the other one.
EntityId createProjectileImpactEffect(vector3df position, f32 duration);

//Causes the missile to track its target and fly towards it.
void missileGoTo(EntityId id, f32 dt);

//Convenience function for tacking on a rigid body component to a projectile.
BulletRigidBodyComponent* addProjectileRBC(EntityId id, btVector3& initForce, btVector3& initVelocity, vector3df& spawn, btQuaternion& initRot);
//Convenience function for tacking on a parent component to a projectile.
ParentComponent* addProjectileParent(EntityId id, EntityId parent);
//Convenience function for tacking on the weapon information to a projectile.
ProjectileInfoComponent* addProjectileInfo(EntityId id, WeaponInfoComponent* wepInfo, vector3df spawnPos);
#endif 