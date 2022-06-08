
#include "GameController.h"
#include "ExplosionSystem.h"
#include <iostream>

void ExplodeAOE(ExplosionComponent* exp)
{
	btVector3 center = irrVecToBt(exp->explosion->getPosition());
	btSphereShape shape(exp->radius);
	btPairCachingGhostObject ghost;
	btTransform transform;
	transform.setOrigin(center);
	ghost.setCollisionShape(&shape);
	ghost.setWorldTransform(transform);
	bWorld->addCollisionObject(&ghost);
	for (u32 i = 0; i < ghost.getNumOverlappingObjects(); ++i) {
		btCollisionObject* obj = ghost.getOverlappingObject(i);

		flecs::entity objId = getIdFromBt(obj);
		if (!objId.is_alive()) continue;
		if (!objId.has<BulletRigidBodyComponent>(objId)) continue;
		auto objRBC = objId.get_mut<BulletRigidBodyComponent>();

		btVector3 dist = objRBC->rigidBody.getCenterOfMassPosition() - center;
		btVector3 dir = dist.normalized();
		f32 distfactor = (exp->radius - dist.length()) / exp->radius;
		if (std::abs(distfactor) > 1.f) {
			distfactor = 1.f;
		}

		objRBC->rigidBody.applyCentralImpulse(dir * (exp->force * distfactor));
		objRBC->rigidBody.applyTorqueImpulse(dir * (exp->force * distfactor / 10.f));
		objRBC->rigidBody.activate(true);

		f32 damage = exp->damage * distfactor;
		if (damage < 0) damage = 0;

		if (!objId.has<DamageTrackingComponent>()) continue;
		auto dmg = objId.get_mut<DamageTrackingComponent>();
		dmg->registerDamageInstance(DamageInstance(INVALID_ENTITY, objId, DAMAGE_TYPE::EXPLOSIVE,
			damage, device->getTimer()->getTime()));
	}
	bWorld->removeCollisionObject(&ghost);
}

void explosionSystem(flecs::iter it, ExplosionComponent* exc)
{
	for (auto i : it) {
		ExplosionComponent* exp = &exc[i];
		if (exp->lifetime == 0 && exp->force > 0) ExplodeAOE(exp);
		exp->lifetime += it.delta_time();
		auto em = exp->explosion->getEmitter();

		f32 completion = (exp->duration - exp->lifetime) / exp->duration;
		if (completion < 0) completion = 0;

		if (exp->light) exp->light->setRadius(150.f * completion);

		em->setMinParticlesPerSecond((u32)(200 * completion));
		em->setMaxParticlesPerSecond((u32)(500 * completion));

		if (exp->lifetime >= exp->duration + 3) {
			exp->explosion->remove();
			if (exp->light) exp->light->remove();
			it.entity(i).destruct();
		}
	}
}

/*
void explosionSystem(f32 dt)
{
	for (auto id : SceneView<ExplosionComponent>(sceneManager->scene))
	{
		auto exp = sceneManager->scene.get<ExplosionComponent>(id);
		if (exp->lifetime == 0 && exp->force > 0) ExplodeAOE(exp);

		exp->lifetime += dt;
		auto em = exp->explosion->getEmitter();
		f32 completion = (exp->duration - exp->lifetime) / exp->duration;
		if (completion < 0) completion = 0;

		if(exp->light) exp->light->setRadius(150.f * completion);

		em->setMinParticlesPerSecond((u32)(200 * completion));
		em->setMaxParticlesPerSecond((u32)(500 * completion));

		if (exp->lifetime >= exp->duration + 3) {
			exp->explosion->remove();
			if(exp->light) exp->light->remove();
			sceneManager->scene.destroyEntity(id);
		}
	}
}
*/