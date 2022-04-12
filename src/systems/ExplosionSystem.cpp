#include "SceneManager.h"
#include "ExplosionSystem.h"
#include <iostream>

void ExplodeAOE(ExplosionComponent* exp, SceneManager* manager)
{
	btVector3 center = irrVecToBt(exp->explosion->getPosition());
	btSphereShape shape(exp->radius);
	btPairCachingGhostObject ghost;
	btTransform transform;
	transform.setOrigin(center);
	ghost.setCollisionShape(&shape);
	ghost.setWorldTransform(transform);
	manager->bulletWorld->addCollisionObject(&ghost);
	for (u32 i = 0; i < ghost.getNumOverlappingObjects(); ++i) {
		btCollisionObject* obj = ghost.getOverlappingObject(i);
		EntityId objId = getIdFromBt(obj);
		if (!manager->scene.entityInUse(objId)) continue;
		auto objRBC = manager->scene.get<BulletRigidBodyComponent>(objId);
		btVector3 dist = center - objRBC->rigidBody.getCenterOfMassPosition();
		btVector3 dir = dist.normalized();
		f32 distfactor = (exp->radius - dist.length()) / exp->radius;
		objRBC->rigidBody.applyCentralImpulse(dir * (exp->force * distfactor));
		objRBC->rigidBody.applyTorqueImpulse(dir * (exp->force * distfactor / 10.f));
		objRBC->rigidBody.activate(true);

		auto hp = manager->scene.get<HealthComponent>(objId);
		if (hp) {
			hp->health -= exp->damage * distfactor;
			//BUG: If the radius isn't large enough but they still collide this will actually ADD health.
			//Need to determine point of contact
		}

	}
	manager->bulletWorld->removeCollisionObject(&ghost);
}

void explosionSystem(SceneManager* manager, f32 dt)
{
	for (auto id : SceneView<ExplosionComponent>(manager->scene))
	{
		auto exp = manager->scene.get<ExplosionComponent>(id);
		if (exp->lifetime == 0 && exp->force > 0) ExplodeAOE(exp, manager);

		exp->lifetime += dt;
		auto em = exp->explosion->getEmitter();
		if (exp->lifetime >= exp->duration) {
			exp->explosion->remove();
			manager->scene.destroyEntity(id);
		}
		f32 completion = (exp->duration - exp->lifetime) / exp->duration;
		em->setMinParticlesPerSecond((u32)(200 * completion));
		em->setMaxParticlesPerSecond((u32)(500 * completion));

	}
}