#include "CollisionCheckingSystem.h"
#include "SceneManager.h"
#include <iostream>

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

	projectileImpact(manager, irr->node->getPosition(), .2f);
	destroyProjectile(manager, projectile);
}

void collisionDamage(SceneManager* manager, EntityId A, EntityId B)
{
	auto rbcA = manager->scene.get<BulletRigidBodyComponent>(A);
	auto rbcB = manager->scene.get<BulletRigidBodyComponent>(B);
	auto hpA = manager->scene.get<HealthComponent>(A);
	auto hpB = manager->scene.get<HealthComponent>(B);

	if (!rbcA || !rbcB) return;

	btScalar velA = rbcA->rigidBody.getLinearVelocity().length2();
	btScalar velB = rbcB->rigidBody.getLinearVelocity().length2();
	btScalar kinetic = 0;
	btScalar minimumVel = 10.f;
	if (velA < minimumVel || velB < minimumVel) return;

	//Any "static" object would have a mass of 0, and would have 0 velocity, so in both cases the static
	//object should never be applying the damage.
	if (velA >= velB) kinetic = (velA * rbcA->rigidBody.getMass()) / 2;
	else kinetic = (velB * rbcB->rigidBody.getMass()) / 2;

	//balancing for later
	kinetic = kinetic / 10000.f;
	if (kinetic < .1f) return;
	if (hpA) hpA->health -= kinetic / 2;
	if (hpB) hpB->health -= kinetic / 2;
}

void collisionCheckingSystem(SceneManager* manager)
{
	int numManifolds = manager->bulletWorld->getDispatcher()->getNumManifolds();

	for (int i = 0; i < numManifolds; ++i) {
		btPersistentManifold* contact = manager->bulletWorld->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* objA = const_cast<btCollisionObject*>(contact->getBody0());
		btCollisionObject* objB = const_cast<btCollisionObject*>(contact->getBody1());
		btRigidBody* rbcA = (btRigidBody*)objA;
		btRigidBody* rbcB = (btRigidBody*)objB;

		EntityId idA = getIdFromBt(objA);
		EntityId idB = getIdFromBt(objB);

		if (idA != INVALID_ENTITY && idB != INVALID_ENTITY) {
			//projectile handling
			auto projA = manager->scene.get<ProjectileInfoComponent>(idA);
			auto projB = manager->scene.get<ProjectileInfoComponent>(idB);
			if (projA && !projB) {
				projectileCollider(manager, idA, idB);
				continue;
			}
			else if (projB && !projA) {
				projectileCollider(manager, idB, idA);
				continue;
			} 
			else if (projB && projA) {
				destroyProjectile(manager, idA);
				destroyProjectile(manager, idB);
				continue;
			}
			//projectile cases have been handled. now for impact damage
			collisionDamage(manager, idA, idB);
		}
	}
}

bool collisionFilterCallback::needBroadphaseCollision(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1) const
{
	btCollisionObject* a = static_cast<btCollisionObject*>(proxy0->m_clientObject);
	btCollisionObject* b = static_cast<btCollisionObject*>(proxy1->m_clientObject);

	EntityId idA = getIdFromBt(a);
	EntityId idB = getIdFromBt(b);
	if (!manager->scene.entityInUse(idA) || !manager->scene.entityInUse(idB)) return true; //something probably went wrong if either of these hits
	//need to check who "owns" these entities
	auto parentA = manager->scene.get<ParentComponent>(idA);
	auto parentB = manager->scene.get<ParentComponent>(idB);
	if (parentA && parentB) { //if they have the same parent (i.e. projectiles) return false
		if (parentA->parentId == parentB->parentId) return false;
	}
	auto projA = manager->scene.get<ProjectileInfoComponent>(idA);
	auto projB = manager->scene.get<ProjectileInfoComponent>(idB);

	if (projA && projB) {
		return false;
	}

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

bool collisionFilterCallback::isProjectileHittingParent(EntityId proj, EntityId other) const
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