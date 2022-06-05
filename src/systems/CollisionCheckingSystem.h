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
#include "btUtils.h"

//Checks all collisions that are currently happening in the scene. This function handles things like projectiles hitting ships,
//and updates health components accordingly.
void collisionCheckingSystem(flecs::iter it);

//Checks two entities: the projectile and whatever got hit by the projectile. Removes the appropriate amount of health.
void projectileCollider(flecs::entity projectile, flecs::entity impacted);

//The callback used by bullet physics to determine when two things need to have collision associated with them. Doesn't do anything out
//of the ordinary - but it will be used to make sure that a ship can't shoot itself, for example, by checking the IDs associated with
//both the projectile entity and the ship entity.
struct broadCallback : public btOverlapFilterCallback
{
	//returns true when pairs need collision
	virtual bool needBroadphaseCollision(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1) const;
	bool isProjectileHittingParent(flecs::entity proj, flecs::entity other) const;//domestic abuse should never be allowed
};



#endif