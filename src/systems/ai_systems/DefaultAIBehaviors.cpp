#include "DefaultAIBehaviors.h"
#include "SceneManager.h"

void defaultIdleBehavior(SceneManager* manager, EntityId id, f32 dt)
{
	auto rbc = manager->scene.get<BulletRigidBodyComponent>(id);
	auto ship = manager->scene.get<ShipComponent>(id);

	btVector3 force = btVector3(0, 0, 0);
	btVector3 torque = btVector3(0, 0, 0);

	btVector3 ang = rbc->rigidBody.getAngularVelocity();
	btVector3 lin = rbc->rigidBody.getLinearVelocity();
	ang.safeNormalize();
	lin.safeNormalize();
	torque -= ang * ship->rotSpeed;
	force -= lin * ship->speed;

	rbc->rigidBody.applyTorqueImpulse(torque * dt);
	rbc->rigidBody.applyCentralImpulse(force * dt);
}

void defaultFleeBehavior(SceneManager* manager, EntityId id, f32 dt)
{
	auto irr = manager->scene.get<IrrlichtComponent>(id);
	auto rbc = manager->scene.get<BulletRigidBodyComponent>(id);
	auto ship = manager->scene.get<ShipComponent>(id);
	auto ai = manager->scene.get<AIComponent>(id);

	auto pursuitIrr = manager->scene.get<IrrlichtComponent>(ai->closestContact);
	vector3df distance = pursuitIrr->node->getPosition() - irr->node->getPosition();
	distance.normalize();
	vector3df targetVector = -distance; //runs away

	btVector3 force = btVector3(0, 0, 0);
	btVector3 torque = btVector3(0, 0, 0);

	force += getForceForward(&rbc->rigidBody, ship);
	torque += getTorqueToDirection(&rbc->rigidBody, ship, irrlichtVectorToBullet(targetVector));

	rbc->rigidBody.applyTorqueImpulse(torque * dt);
	rbc->rigidBody.applyCentralImpulse(force * dt);
}