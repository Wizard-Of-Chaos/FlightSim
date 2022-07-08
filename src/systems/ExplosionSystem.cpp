
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
		if (!objId.has<BulletRigidBodyComponent>()) continue;
		auto objRBC = objId.get_mut<BulletRigidBodyComponent>();

		btVector3 dist = objRBC->rigidBody->getCenterOfMassPosition() - center;
		btVector3 dir = dist.normalized();
		f32 distfactor = (exp->radius - dist.length()) / exp->radius;
		if (std::abs(distfactor) > 1.f) {
			distfactor = 1.f;
		}

		objRBC->rigidBody->applyCentralImpulse(dir * (exp->force * distfactor));
		objRBC->rigidBody->applyTorqueImpulse(dir * (exp->force * distfactor / 10.f));
		objRBC->rigidBody->activate(true);

		f32 damage = exp->damage * distfactor;
		if (damage < 0) damage = 0;

		if (!objId.has<DamageTrackingComponent>()) continue;
		auto dmg = objId.get_mut<DamageTrackingComponent>();
		dmg->registerDamageInstance(DamageInstance(INVALID_ENTITY, objId, DAMAGE_TYPE::EXPLOSIVE,
			damage, device->getTimer()->getTime()));
	}
	bWorld->removeCollisionObject(&ghost);
	exp->hasExploded = true;
}

void explosionSystem(flecs::iter it, ExplosionComponent* exc, IrrlichtComponent* irrc)
{
	for (auto i : it) {
		ExplosionComponent* exp = &exc[i];
		IrrlichtComponent* irr = &irrc[i];
		if (!exp->hasExploded && exp->force > 0) ExplodeAOE(exp);
		exp->lifetime += it.delta_time();
		auto em = exp->explosion->getEmitter();

		f32 completion = (exp->duration - exp->lifetime) / exp->duration;

		if (completion < 0) completion = 0;

		if (exp->light) exp->light->setRadius(150.f * completion);

		if (irr->node) {
			vector3df scale = irr->node->getScale() * 1.04f;
			irr->node->setScale(scale);
		}
		em->setMinParticlesPerSecond((u32)(200 * completion));
		em->setMaxParticlesPerSecond((u32)(500 * completion));

		if (exp->lifetime >= exp->duration + 3) {
			exp->explosion->remove();
			if (exp->light) exp->light->remove();
			destroyObject(it.entity(i));
		}
	}
}