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

void collisionCheckingSystem(SceneManager* manager)
{
	int numManifolds = manager->bulletWorld->getDispatcher()->getNumManifolds();

	for (int i = 0; i < numManifolds; ++i) {
		btPersistentManifold* contact = manager->bulletWorld->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* objA = const_cast<btCollisionObject*>(contact->getBody0());
		btCollisionObject* objB = const_cast<btCollisionObject*>(contact->getBody1());

		EntityId idA = getIdFromBt(objA);
		EntityId idB = getIdFromBt(objB);

		ProjectileInfoComponent* projA = nullptr;
		ProjectileInfoComponent* projB = nullptr;
		HealthComponent* hpA = nullptr;
		HealthComponent* hpB = nullptr;

		if (idA != INVALID_ENTITY) {
			projA = manager->scene.get<ProjectileInfoComponent>(idA);
			hpA = manager->scene.get<HealthComponent>(idA);
		}
		
		if (idB != INVALID_ENTITY) {
			projB = manager->scene.get<ProjectileInfoComponent>(idB);
			hpB = manager->scene.get<HealthComponent>(idB);
		}

		//health updates on collision
		if (hpA) {
			if (projB) {
				hpA->health -= projB->damage;
			} // the "else" here should be checking mass and velocity to apply damage if you bonk something super hard
		}
		if (hpB) {
			if (projA) {
				hpB->health -= projB->damage;
			}
		}

		//projectile destruction on collision
		if (projA) {
			destroyProjectile(manager, idA);
		}
		if (projB) {
			destroyProjectile(manager, idB);
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