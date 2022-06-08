#include "SensorUpdateSystem.h"
#include "GameController.h"
#include <iostream>

std::vector<ContactInfo> getContacts(BulletRigidBodyComponent* rbc, SensorComponent* sens, FactionComponent* fac)
{
	std::vector<ContactInfo> ret;

	btSphereShape shape(sens->detectionRadius);
	btPairCachingGhostObject ghost; ///ooooooooooooooooooooooooo!
	btTransform transform;
	transform.setOrigin(rbc->rigidBody.getCenterOfMassPosition());
	ghost.setCollisionShape(&shape);
	ghost.setWorldTransform(transform);
	bWorld->addCollisionObject(&ghost);


	btVector3 closeHostileDist(0, 0, 0);
	btVector3 closeFriendlyDist(0, 0, 0);
	btVector3 closeDist(0, 0, 0);

	for (u32 i = 0; i < ghost.getNumOverlappingObjects(); ++i) {
		btCollisionObject* obj = ghost.getOverlappingObject(i);
		if (obj == &rbc->rigidBody) continue;

		flecs::entity objId = getIdFromBt(obj);
		if (!objId.is_alive()) continue;

		if (!objId.has<BulletRigidBodyComponent>() || !objId.has<FactionComponent>()) continue; //throw out anything without a rigid body comp and a faction comp
		auto objRBC = objId.get<BulletRigidBodyComponent>();
		auto objFac = objId.get<FactionComponent>();
		ContactInfo info = { objId, objRBC, objFac };

		btVector3 dist = objRBC->rigidBody.getCenterOfMassPosition() - rbc->rigidBody.getCenterOfMassPosition();
		btScalar len = dist.length2();
		//update closest hostile, friendly, and general contacts
		if (len > closeDist.length2()) {
			closeDist = dist;
			sens->closestContact = objId;
		}

		if (len > closeHostileDist.length2() && objFac->isHostile(fac)) {
			closeHostileDist = dist;
			sens->closestHostileContact = objId;
		}
		if (len > closeFriendlyDist.length2() && objFac->isFriendly(fac)) {
			closeFriendlyDist = dist;
			sens->closestFriendlyContact = objId;
		}
		ret.push_back(ContactInfo{objId, objRBC, objFac});
	}

	bWorld->removeCollisionObject(&ghost); //get rid of the sphere used to check
	return ret;
}

void sensorSystem(flecs::iter it, BulletRigidBodyComponent* rbcs, SensorComponent* sns, FactionComponent* fcs)
{
	for (auto i : it) {
		auto sens = &sns[i];
		auto fac = &fcs[i];
		auto rbc = &rbcs[i];
		sens->timeSinceLastUpdate += it.delta_time();
		if (sens->timeSinceLastUpdate >= sens->updateInterval) {
			sens->contacts = getContacts(rbc, sens, fac);
			sens->timeSinceLastUpdate = 0;
		}
		if (sens->targetContact != INVALID_ENTITY) {
			sens->timeSelected += it.delta_time();
		}
		else {
			sens->timeSelected = 0;
		}
	}
}

/*
void sensorSystem(f32 dt)
{
	for (EntityId id : SceneView<BulletRigidBodyComponent, SensorComponent, FactionComponent>(sceneManager->scene)) {
		auto sens = sceneManager->scene.get<SensorComponent>(id);
		auto fac = sceneManager->scene.get<FactionComponent>(id);
		auto rbc = sceneManager->scene.get<BulletRigidBodyComponent>(id);
		sens->timeSinceLastUpdate += dt;
		if (sens->timeSinceLastUpdate >= sens->updateInterval) {
			sens->contacts = getContacts(rbc, sens, fac);
			sens->timeSinceLastUpdate = 0;
		}
		if (sens->targetContact != INVALID_ENTITY) {
			sens->timeSelected += dt;
		} else {
			sens->timeSelected = 0;
		}
	}
}
*/