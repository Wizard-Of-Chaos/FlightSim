#pragma once
#ifndef PROJECTILEUTILS_H
#define PROJECTILEUTILS_H
#include "BaseHeader.h"
#include "WeaponInfoComponent.h"
#include "IrrlichtComponent.h"
#include "BulletRigidBodyComponent.h"
#include "MissileComponent.h"
#include "ShipMovementUtils.h"

//tag for projectiles that have been fired
struct FiredBy {};

//Creates a projectile entity and shoots it in the indicated direction. Assigns information to the projectile (range, speed, etc)
//based on the weapon that shot it. Returns the ID of the projectile.
flecs::entity createProjectileEntity(vector3df spawnPos, vector3df direction, flecs::entity weaponId);
//Removes the given projectile from the scene.
void destroyProjectile(flecs::entity projectile);

//Adds on the info for a plasma projectile as well as particle effects.
void createPlasmaProjectile(flecs::entity projId, vector3df dir, vector3df spawn);

//Adds on the info for a missile projectile as well as particle effects.
void createMissileProjectile(flecs::entity projId, vector3df dir, vector3df spawn);

//Adds on the info for a kinetic projectile as well as particle effects.
void createKineticProjectile(flecs::entity projId, vector3df dir, vector3df spawn);

//Creates an explosion at the impact point of a projectile. Way smaller than the other one.
flecs::entity createProjectileImpactEffect(vector3df position, f32 duration);

//Causes the missile to track its target and fly towards it.
void missileGoTo(flecs::entity id, f32 dt);

//Convenience function for tacking on a rigid body component to a projectile.
BulletRigidBodyComponent* addProjectileRBC(flecs::entity id, btVector3& initForce, btVector3& initVelocity, vector3df& spawn, btQuaternion& initRot);
//Convenience function for tacking on the weapon information to a projectile.
ProjectileInfoComponent addProjectileInfo(WeaponInfoComponent* wepInfo, vector3df spawnPos);
#endif 