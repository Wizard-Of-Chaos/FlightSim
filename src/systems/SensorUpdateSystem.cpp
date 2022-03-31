#include "SensorUpdateSystem.h"
#include "SceneManager.h"
#include "GameController.h"
#include <iostream>

std::vector<ContactInfo> getContacts(SceneManager* manager, BulletRigidBodyComponent* rbc, SensorComponent* sens, FactionComponent* fac)
{
	std::vector<ContactInfo> ret;

	btSphereShape shape(sens->detectionRadius);
	btPairCachingGhostObject ghost; ///ooooooooooooooooooooooooo!
	btTransform transform;
	transform.setOrigin(rbc->rigidBody.getCenterOfMassPosition());
	ghost.setCollisionShape(&shape);
	ghost.setWorldTransform(transform);
	manager->bulletWorld->addCollisionObject(&ghost);


	btVector3 closeHostileDist(0, 0, 0);
	btVector3 closeFriendlyDist(0, 0, 0);
	btVector3 closeDist(0, 0, 0);

	for (u32 i = 0; i < ghost.getNumOverlappingObjects(); ++i) {
		btCollisionObject* obj = ghost.getOverlappingObject(i);
		if (obj == &rbc->rigidBody) continue;

		EntityId objId = getIdFromBt(obj);
		if (!manager->scene.entityInUse(objId)) continue;

		auto objRBC = manager->scene.get<BulletRigidBodyComponent>(objId);
		auto objFac = manager->scene.get<FactionComponent>(objId);
		if (!objRBC || !objFac) continue; //throw out anything without a rigid body comp and a faction comp

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

	manager->bulletWorld->removeCollisionObject(&ghost); //get rid of the sphere used to check
	return ret;
}

void sensorSystem(SceneManager* manager, f32 dt)
{
	for (EntityId id : SceneView<BulletRigidBodyComponent, SensorComponent, FactionComponent>(manager->scene)) {
		auto sens = manager->scene.get<SensorComponent>(id);
		auto fac = manager->scene.get<FactionComponent>(id);
		auto rbc = manager->scene.get<BulletRigidBodyComponent>(id);
		sens->timeSinceLastUpdate += dt;
		if (sens->timeSinceLastUpdate >= sens->updateInterval) {
			sens->contacts = getContacts(manager, rbc, sens, fac);
			sens->timeSinceLastUpdate = 0;
		}
	}
}