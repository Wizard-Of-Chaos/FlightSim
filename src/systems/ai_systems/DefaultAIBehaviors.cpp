#include "DefaultAIBehaviors.h"
#include "SceneManager.h"

void defaultIdleBehavior(SceneManager* manager, EntityId id, f32 dt)
{
	auto rbc = manager->scene.get<BulletRigidBodyComponent>(id);
	auto ship = manager->scene.get<ShipComponent>(id);

	btVector3 force = btVector3(0, 0, 0);
	btVector3 torque = btVector3(0, 0, 0);

	force += getForceToStopLinearVelocity(&rbc->rigidBody, ship);
	torque += getTorqueToStopAngularVelocity(&rbc->rigidBody, ship);

	rbc->rigidBody.applyTorqueImpulse(torque * dt);
	rbc->rigidBody.applyCentralImpulse(force * dt);
}

void defaultFleeBehavior(SceneManager* manager, EntityId id, f32 dt)
{
	auto irr = manager->scene.get<IrrlichtComponent>(id);
	auto rbc = manager->scene.get<BulletRigidBodyComponent>(id);
	auto ship = manager->scene.get<ShipComponent>(id);
	auto ai = manager->scene.get<AIComponent>(id);

	auto fleeIrr = manager->scene.get<IrrlichtComponent>(ai->closestContact);
	vector3df distance = fleeIrr->node->getPosition() - irr->node->getPosition();
	distance.normalize();
	vector3df targetVector = -distance; //runs away

	btVector3 force = btVector3(0, 0, 0);
	btVector3 torque = btVector3(0, 0, 0);

	force += getForceForward(&rbc->rigidBody, ship);

	smoothTurnToDirection(&rbc->rigidBody, ship, irrlichtVectorToBullet(targetVector), dt);
	rbc->rigidBody.applyCentralImpulse(force * dt);
}

void defaultPursuitBehavior(SceneManager* manager, EntityId id, f32 dt)
{
	auto irr = manager->scene.get<IrrlichtComponent>(id);
	auto rbc = manager->scene.get<BulletRigidBodyComponent>(id);
	auto ship = manager->scene.get<ShipComponent>(id);
	auto ai = manager->scene.get<AIComponent>(id);

	auto pursuitIrr = manager->scene.get<IrrlichtComponent>(ai->closestContact);
	vector3df tailPos = getNodeBackward(pursuitIrr->node) * 30.f;
	goToPoint(&rbc->rigidBody, ship, irrlichtVectorToBullet(tailPos), dt);
	//get behind the ship, then try and match velocity while turning toward it
}