#pragma once

#ifndef COLLISIONCHECKINGSYSTEM_H
#define COLLISIONCHECKINGSYSTEM_H
#include "BaseHeader.h"
#include "WeaponInfoComponent.h"
#include "IrrlichtComponent.h"
#include "BulletRigidBodyComponent.h"
#include "ProjectileSystem.h"
#include "HealthComponent.h"

class SceneManager;

void collisionCheckingSystem(SceneManager* manager);
EntityId getIdFromBt(btCollisionObject* object);

struct collisionFilterCallback : public btOverlapFilterCallback
{
	//returns true when pairs need collision
	virtual bool needBroadphaseCollision(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1);
	SceneManager* manager;
};

#endif