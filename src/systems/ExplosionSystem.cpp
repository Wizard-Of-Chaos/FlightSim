#include "SceneManager.h"
#include "GameController.h"
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
		btVector3 dist = objRBC->rigidBody.getCenterOfMassPosition() - center;
		btVector3 dir = dist.normalized();
		f32 distfactor = (exp->radius - dist.length()) / exp->radius;
		if (std::abs(distfactor) > 1.f) {
			distfactor = 1.f;
		}

		objRBC->rigidBody.applyCentralImpulse(dir * (exp->force * distfactor));
		objRBC->rigidBody.applyTorqueImpulse(dir * (exp->force * distfactor / 10.f));
		objRBC->rigidBody.activate(true);

		auto dmg = manager->scene.get<DamageTrackingComponent>(objId);
		if (dmg) {
			dmg->registerDamageInstance(DamageInstance(INVALID_ENTITY, objId, DAMAGE_TYPE::EXPLOSIVE,
				exp->damage * distfactor, manager->controller->device->getTimer()->getTime()));

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
		/*
		if (exp->lifetime >= exp->duration) {
			exp->explosion->remove();
			exp->light->remove();
			manager->scene.destroyEntity(id);
		}
		*/
		f32 completion = (exp->duration - exp->lifetime) / exp->duration;
		if (completion < 0) completion = 0;

		if(exp->light) exp->light->setRadius(150.f * completion);

		em->setMinParticlesPerSecond((u32)(200 * completion));
		em->setMaxParticlesPerSecond((u32)(500 * completion));

		if (exp->lifetime >= exp->duration + 3) {
			exp->explosion->remove();
			if(exp->light) exp->light->remove();
			manager->scene.destroyEntity(id);
		}
	}
}