#pragma once

#ifndef COLLISIONCHECKINGSYSTEM_H
#define COLLISIONCHECKINGSYSTEM_H
#include "BaseHeader.h"
#include "WeaponInfoComponent.h"
#include "IrrlichtComponent.h"
#include "BulletRigidBodyComponent.h"
#include "ProjectileSystem.h"
#include "HealthComponent.h"
#include "ShieldComponent.h"
#include "ProjectileUtils.h"

class SceneManager;

//Checks all collisions that are currently happening in the scene. This function handles things like projectiles hitting ships,
//and updates health components accordingly.
void collisionCheckingSystem(SceneManager* manager);
//Convenience function to get the ID from a bullet collision object.
EntityId getIdFromBt(btCollisionObject* object);

//Checks two entities: the projectile and whatever got hit by the projectile. Removes the appropriate amount of health.
void projectileCollider(SceneManager* manager, EntityId projectile, EntityId impacted);

//The callback used by bullet physics to determine when two things need to have collision associated with them. Doesn't do anything out
//of the ordinary - but it will be used to make sure that a ship can't shoot itself, for example, by checking the IDs associated with
//both the projectile entity and the ship entity.
struct collisionFilterCallback : public btOverlapFilterCallback
{
	//returns true when pairs need collision
	virtual bool needBroadphaseCollision(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1);
	bool isProjectileHittingParent(EntityId proj, EntityId other); //domestic abuse should never be allowed
	SceneManager* manager;
};

#endif