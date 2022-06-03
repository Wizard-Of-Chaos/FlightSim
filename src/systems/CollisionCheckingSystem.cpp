#include "CollisionCheckingSystem.h"
#include "SceneManager.h"
#include "GameController.h"
#include "GameStateController.h"
#include "WeaponUtils.h"
#include <iostream>

void projectileCollider(EntityId projectile, EntityId impacted)
{
	auto dmg = sceneManager->scene.get<DamageTrackingComponent>(impacted);
	auto proj = sceneManager->scene.get<ProjectileInfoComponent>(projectile);
	auto irr = sceneManager->scene.get<IrrlichtComponent>(projectile);

	/*
	if (proj->type == WEP_MISSILE) {
		explode(irr->node->getAbsolutePosition(), 1.f, 1.f, 20.f, proj->damage, 100.f);
	}
	if (proj->type == WEP_PHYS_IMPULSE) {
		gameController->registerSoundInstance(impacted, stateController->assets.getSoundAsset("physicsBlastSound"), 1.f, 200.f);
		explode(irr->node->getAbsolutePosition(), 1.f, 1.f, 80.f, proj->damage, 500.f);
	}
	*/
	handleProjectileImpact(projectile, impacted);
	if (dmg) dmg->registerDamageInstance(DamageInstance(projectile, impacted, proj->dmgtype, proj->damage, device->getTimer()->getTime()));
	createProjectileImpactEffect(irr->node->getPosition(), .2f);
	destroyProjectile(projectile);
}

void collisionDamage(EntityId A, EntityId B)
{
	auto rbcA = sceneManager->scene.get<BulletRigidBodyComponent>(A);
	auto rbcB = sceneManager->scene.get<BulletRigidBodyComponent>(B);
	auto dmgA = sceneManager->scene.get<DamageTrackingComponent>(A);
	auto dmgB = sceneManager->scene.get<DamageTrackingComponent>(B);

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
	kinetic = kinetic / 8000.f;
	if (kinetic < .1f) return;
	if (dmgA) dmgA->registerDamageInstance(DamageInstance(B, A, DAMAGE_TYPE::IMPACT, kinetic / 2, device->getTimer()->getTime()));
	if (dmgB) dmgB->registerDamageInstance(DamageInstance(A, B, DAMAGE_TYPE::IMPACT, kinetic / 2, device->getTimer()->getTime()));
}

void collisionCheckingSystem()
{
	int numManifolds = bWorld->getDispatcher()->getNumManifolds();

	for (int i = 0; i < numManifolds; ++i) {
		btPersistentManifold* contact = bWorld->getDispatcher()->getManifoldByIndexInternal(i);
		int numContacts = contact->getNumContacts();
		btCollisionObject* objA = const_cast<btCollisionObject*>(contact->getBody0());
		btCollisionObject* objB = const_cast<btCollisionObject*>(contact->getBody1());

		btRigidBody* rbcA = (btRigidBody*)objA;
		btRigidBody* rbcB = (btRigidBody*)objB;

		EntityId idA = getIdFromBt(objA);
		EntityId idB = getIdFromBt(objB);
		if (!sceneManager->scene.entityInUse(idA) || !sceneManager->scene.entityInUse(idB)) return;
		auto projA = sceneManager->scene.get<ProjectileInfoComponent>(idA);
		auto projB = sceneManager->scene.get<ProjectileInfoComponent>(idB);
		if (projA && !projB) {
			projectileCollider(idA, idB);
			continue;
		}
		else if (projB && !projA) {
			projectileCollider(idB, idA);
			continue;
		}
		else if (projB && projA) {
			destroyProjectile(idA);
			destroyProjectile(idB);
			continue;
		}
		for (int j = 0; j < numContacts; ++j) {
			if (contact->getContactPoint(j).getDistance() >= 0.f) continue;

			if (sceneManager->scene.entityInUse(idA) && sceneManager->scene.entityInUse(idB)) {
				//projectile cases have been handled. now for impact damage
				collisionDamage(idA, idB);
			}
		}
	}
}

bool broadCallback::needBroadphaseCollision(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1) const
{
	btCollisionObject* a = static_cast<btCollisionObject*>(proxy0->m_clientObject);
	btCollisionObject* b = static_cast<btCollisionObject*>(proxy1->m_clientObject);

	auto entityA = getIdFromBt(a);
	auto entityB = getIdFromBt(b);
	if (!entityA.is_valid() || !entityB.is_valid()) return true; //something probably went wrong if either of these hits
	//need to check who "owns" these entities
	auto parentA = sceneManager->scene.get<ParentComponent>(idA);
	auto parentB = sceneManager->scene.get<ParentComponent>(idB);
	if (parentA && parentB) { //if they have the same parent (i.e. projectiles) return false
		if (parentA->parentId == parentB->parentId) return false;
	}
	auto projA = sceneManager->scene.get<ProjectileInfoComponent>(idA);
	auto projB = sceneManager->scene.get<ProjectileInfoComponent>(idB);

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

bool broadCallback::isProjectileHittingParent(EntityId proj, EntityId other) const
{
	auto wepParent = sceneManager->scene.get<ParentComponent>(proj);
	if (!wepParent) return true;
	if (!sceneManager->scene.entityInUse(wepParent->parentId)) return true; //set up like this because im not sure if checking them both in the same if will throw a segfault
	auto shipParent = sceneManager->scene.get<ParentComponent>(wepParent->parentId);
	if (!shipParent) return true;
	if (!sceneManager->scene.entityInUse(shipParent->parentId)) return true;

	if (other == shipParent->parentId) return false;

	return true;

}