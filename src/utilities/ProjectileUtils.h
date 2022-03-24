#pragma once
#ifndef PROJECTILEUTILS_H
#define PROJECTILEUTILS_H
#include "BaseHeader.h"
#include "WeaponInfoComponent.h"
#include "IrrlichtComponent.h"
#include "BulletRigidBodyComponent.h"
#include "MissileComponent.h"
#include "ShipMovementUtils.h"



class SceneManager;
class GameController;

//Creates a projectile entity and shoots it in the indicated direction. Assigns information to the projectile (range, speed, etc)
//based on the weapon that shot it. Returns the ID of the projectile.
EntityId createProjectileEntity(SceneManager* manager, vector3df spawnPos, vector3df direction, EntityId weaponId);
//Removes the given projectile from the scene.
void destroyProjectile(SceneManager* manager, EntityId projectile);

//Adds on the info for a plasma projectile as well as particle effects.
void createPlasmaProjectile(SceneManager* manager, EntityId projId, vector3df dir, vector3df spawn);

void createMissileProjectile(SceneManager* manager, EntityId projId, MissileInfoComponent* missInfo, vector3df dir, vector3df spawn);

//Creates an explosion at the impact point of a projectile. Way smaller than the other one.
EntityId projectileImpact(SceneManager* manager, vector3df position, f32 duration);

void missileGoTo(SceneManager* manager, EntityId id, f32 dt);
#endif 