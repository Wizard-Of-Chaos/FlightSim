#include "CollisionCheckingSystem.h"
#include "SceneManager.h"
#include <iostream>

EntityId getIdFromBt(btCollisionObject* object)
{
	EntityIndex ind = object->getUserIndex();
	EntityVersion ver = object->getUserIndex2();
	int hasEnt = object->getUserIndex3();
	EntityId id = INVALID_ENTITY;
	if (hasEnt) {
		id = createEntityId(ind, ver);
	}
	return id;
}

void projectileCollider(SceneManager* manager, EntityId projectile, EntityId impacted)
{
	auto hp = manager->scene.get<HealthComponent>(impacted);
	auto proj = manager->scene.get<ProjectileInfoComponent>(projectile);
	auto irr = manager->scene.get<IrrlichtComponent>(projectile);
	if (hp) {
		hp->health -= proj->damage;
	}
	projectileImpact(manager, irr->node->getPosition(), .2f);
	destroyProjectile(manager, projectile);
}

void collisionCheckingSystem(SceneManager* manager)
{
	int numManifolds = manager->bulletWorld->getDispatcher()->getNumManifolds();

	for (int i = 0; i < numManifolds; ++i) {
		btPersistentManifold* contact = manager->bulletWorld->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* objA = const_cast<btCollisionObject*>(contact->getBody0());
		btCollisionObject* objB = const_cast<btCollisionObject*>(contact->getBody1());

		EntityId idA = getIdFromBt(objA);
		EntityId idB = getIdFromBt(objB);

		if (idA != INVALID_ENTITY && idB != INVALID_ENTITY) {
			auto projA = manager->scene.get<ProjectileInfoComponent>(idA);
			auto projB = manager->scene.get<ProjectileInfoComponent>(idB);
			if (projA) projectileCollider(manager, idA, idB);
			if (projB) projectileCollider(manager, idB, idA);
		}
	}
}

bool collisionFilterCallback::needBroadphaseCollision(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1)
{
	btCollisionObject* a = static_cast<btCollisionObject*>(proxy0->m_clientObject);
	btCollisionObject* b = static_cast<btCollisionObject*>(proxy1->m_clientObject);

	EntityId idA = getIdFromBt(a);
	EntityId idB = getIdFromBt(b);
	//need to check who "owns" these entities
}