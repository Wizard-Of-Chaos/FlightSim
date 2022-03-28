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
	auto shield = manager->scene.get<ShieldComponent>(impacted);
	auto proj = manager->scene.get<ProjectileInfoComponent>(projectile);
	auto irr = manager->scene.get<IrrlichtComponent>(projectile);
	if (hp) {
		f32 overflow = 0;
		if (shield) {
			shield->shields -= proj->damage;
			if (shield->shields <= 0) {
				overflow += -shield->shields;
				shield->shields = 0;
			}
			shield->timeSinceLastHit = 0;
		} else {
			overflow = proj->damage;
		}
		hp->health -= overflow;
	}
	/*
	std::cout << "Impact! ";
	if (shield) std::cout << "Shields: " << shield->shields;
	if (hp) std::cout << " Health: " << hp->health;
	std::cout << std::endl;
	*/
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
	auto parentA = manager->scene.get<ParentComponent>(idA);
	auto parentB = manager->scene.get<ParentComponent>(idB);
	if (parentA && parentB) { //if they have the same parent (i.e. projectiles) return false
		if (parentA->parentId == parentB->parentId) return false;
	}
	auto projA = manager->scene.get<ProjectileInfoComponent>(idA);
	auto projB = manager->scene.get<ProjectileInfoComponent>(idB);

	//if one is a projectile and the other isn't, needs to check whether it hit its parent - if so throw it out
	if (projA && !projB) {
		return isProjectileHittingParent(idA, idB);
	}
	if (!projA && projB) {
		return isProjectileHittingParent(idB, idA);
	}

	//in all other scenarios return true, we need the collision
	return true;
}

bool collisionFilterCallback::isProjectileHittingParent(EntityId proj, EntityId other)
{
	auto wepParent = manager->scene.get<ParentComponent>(proj);
	if (!wepParent) return true;
	if (!manager->scene.entityInUse(wepParent->parentId)) return true; //set up like this because im not sure if checking them both in the same if will throw a segfault
	auto shipParent = manager->scene.get<ParentComponent>(wepParent->parentId);
	if (!shipParent) return true;
	if (!manager->scene.entityInUse(shipParent->parentId)) return true;

	if (other == shipParent->parentId) return false;

	return true;

}