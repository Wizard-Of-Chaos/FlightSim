#include "CollisionCheckingSystem.h"
#include "GameController.h"
#include "GameStateController.h"
#include "WeaponUtils.h"
#include <iostream>

void projectileCollider(flecs::entity projectile, flecs::entity impacted)
{
	if (!impacted.has<DamageTrackingComponent>()) return;
	auto dmg = impacted.get_mut<DamageTrackingComponent>();
	auto proj = projectile.get<ProjectileInfoComponent>();
	auto irr = projectile.get<IrrlichtComponent>();

	handleProjectileImpact(projectile, impacted);
	if (dmg) dmg->registerDamageInstance(DamageInstance(projectile, impacted, proj->dmgtype, proj->damage, device->getTimer()->getTime()));
	createProjectileImpactEffect(irr->node->getPosition(), .2f);
	destroyProjectile(projectile);
}

void collisionDamage(flecs::entity A, flecs::entity B)
{
	if (!A.has<BulletRigidBodyComponent>() || !B.has<BulletRigidBodyComponent>()) return;

	auto rbcA = A.get<BulletRigidBodyComponent>();
	auto rbcB = B.get<BulletRigidBodyComponent>();

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
	if (A.has<DamageTrackingComponent>()) {
		A.get_mut<DamageTrackingComponent>()->registerDamageInstance(DamageInstance(B, A, DAMAGE_TYPE::IMPACT, kinetic / 2, device->getTimer()->getTime()));
	}
	if (B.has<DamageTrackingComponent>()) {
		B.get_mut<DamageTrackingComponent>()->registerDamageInstance(DamageInstance(A, B, DAMAGE_TYPE::IMPACT, kinetic / 2, device->getTimer()->getTime()));
	}
}

void collisionCheckingSystem(flecs::iter it)
{
	int numManifolds = bWorld->getDispatcher()->getNumManifolds();

	for (int i = 0; i < numManifolds; ++i) {
		btPersistentManifold* contact = bWorld->getDispatcher()->getManifoldByIndexInternal(i);
		int numContacts = contact->getNumContacts();
		btCollisionObject* objA = const_cast<btCollisionObject*>(contact->getBody0());
		btCollisionObject* objB = const_cast<btCollisionObject*>(contact->getBody1());

		btRigidBody* rbcA = (btRigidBody*)objA;
		btRigidBody* rbcB = (btRigidBody*)objB;

		flecs::entity idA = getIdFromBt(objA);
		flecs::entity idB = getIdFromBt(objB);
		if (!idA.is_alive()|| !idB.is_alive()) return;
		bool projA = idA.has<ProjectileInfoComponent>();
		bool projB = idB.has<ProjectileInfoComponent>();

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

			if (idA.is_alive() && idB.is_alive()) {
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

	if (entityA.get_object<FiredBy>().id() == entityB.get_object<FiredBy>().id()) return false; //if they have the same parent return false

	if (entityA.has<ProjectileInfoComponent>() && entityB.has<ProjectileInfoComponent>()) { //if they're both projectiles return false
		return false;
	}

	//if one is a projectile and the other isn't, needs to check whether it hit its parent - if so throw it out
	if (entityA.has<ProjectileInfoComponent>() && !entityB.has<ProjectileInfoComponent>()) {
		return isProjectileHittingParent(entityA, entityB);
	}
	if (!entityA.has<ProjectileInfoComponent>() && entityB.has<ProjectileInfoComponent>()) {
		return isProjectileHittingParent(entityB, entityA);
	}

	//in all other scenarios return true, we need the collision
	return true;
}

bool broadCallback::isProjectileHittingParent(flecs::entity proj, flecs::entity other) const
{
	//to-do: set it up so that you can't actually shoot yourself (or maybe leave it as a toggle?)
	return true;
}