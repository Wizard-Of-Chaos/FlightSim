#include "SensorUpdateSystem.h"
#include "SceneManager.h"
#include "GameController.h"

void contactDistanceUpdate(SceneManager* manager, ISceneNode* self, EntityId contact, SensorComponent* sensor, vector3df& closestDist)
{
	if (contact != INVALID_ENTITY) {
		auto irr = manager->scene.get<IrrlichtComponent>(contact);
		if (!irr) return;
		vector3df dist = irr->node->getPosition() - self->getPosition();
		closestDist = dist;
	}
}

void sensorUpdateSystem(SceneManager* manager, f32 dt) //the sensor component does not and SHOULD not pick an active selection; however it can null-out the current one
{
	for (EntityId id : SceneView<IrrlichtComponent, SensorComponent, FactionComponent>(manager->scene)) {
		auto irr = manager->scene.get<IrrlichtComponent>(id);
		auto sensor = manager->scene.get<SensorComponent>(id);
		auto fac = manager->scene.get<FactionComponent>(id);

		if (sensor->targetContact != INVALID_ENTITY) {
			auto selectedIrr = manager->scene.get<IrrlichtComponent>(sensor->targetContact);
			vector3df targetDistance = selectedIrr->node->getPosition() - irr->node->getPosition();
			if (targetDistance.getLength() > sensor->detectionRadius) {
				sensor->targetContact = INVALID_ENTITY; //the actual removal from the list of contacts happens further down the chain
			}
			sensor->timeSelected += dt;
		} else {
			sensor->timeSelected = 0;
		}

		for (EntityId checkId : SceneView<FactionComponent, IrrlichtComponent>(manager->scene)) {
			if (checkId == id) continue;
			//checks everything in the scene to see how close it is; if it's inside the detection radius it puts it into contacts
			auto irrCheck = manager->scene.get<IrrlichtComponent>(checkId);
			vector3df distance = irrCheck->node->getPosition() - irr->node->getPosition();
			if (distance.getLength() <= sensor->detectionRadius) { 
				if (sensor->contacts.binary_search(checkId) == -1) {
					sensor->contacts.push_back(checkId);
				}
			}
		}
		vector3df closestContactDistance(0, 0, 0);
		vector3df closestHostileDistance(0, 0, 0);
		vector3df closestFriendlyDistance(0, 0, 0);
		
		//update closest hostile, friendly, and general-closest contacts
		contactDistanceUpdate(manager, irr->node, sensor->closestContact, sensor, closestContactDistance);
		contactDistanceUpdate(manager, irr->node, sensor->closestHostileContact, sensor, closestHostileDistance);
		contactDistanceUpdate(manager, irr->node, sensor->closestFriendlyContact, sensor, closestFriendlyDistance);

		for (unsigned int i = 0; i < sensor->contacts.size(); ++i) {
			EntityId checkId = sensor->contacts[i];
			if (!manager->scene.entityInUse(checkId)) { //if the entity no longer exists, remove it from the contact list
				sensor->contacts[i] = INVALID_ENTITY;
				if (sensor->closestContact == checkId) sensor->closestContact = INVALID_ENTITY;
				if (sensor->closestHostileContact == checkId) sensor->closestHostileContact = INVALID_ENTITY;
				if (sensor->closestFriendlyContact == checkId) sensor->closestFriendlyContact = INVALID_ENTITY;
				continue;
			}
			auto irrCheck = manager->scene.get<IrrlichtComponent>(checkId);

			vector3df distance = irrCheck->node->getPosition() - irr->node->getPosition();

			//Too much repetition: move into other functions like contactDistanceUpdate
			if (distance.getLength() > sensor->detectionRadius) {
				if (sensor->closestContact == checkId) {
					sensor->closestContact = INVALID_ENTITY;
					closestContactDistance = vector3df(0, 0, 0);
				}
				if (sensor->closestFriendlyContact == checkId) {
					sensor->closestFriendlyContact = INVALID_ENTITY;
					closestFriendlyDistance = vector3df(0, 0, 0);
				}
				if (sensor->closestHostileContact == checkId) {
					sensor->closestHostileContact = INVALID_ENTITY;
					closestHostileDistance = vector3df(0, 0, 0);
				}

				sensor->contacts.erase(i);
				continue;
			}

			if ((distance.getLength() < closestContactDistance.getLength()) || (closestContactDistance.getLength() == 0)) {
				sensor->closestContact = sensor->contacts[i];
				closestContactDistance = distance;
			}

			auto checkFac = manager->scene.get<FactionComponent>(checkId);
			if (!checkFac) continue;

			if (checkFac->isHostile(fac)) {
				if ((closestHostileDistance.getLength() == 0) || (distance.getLength() < closestHostileDistance.getLength())) {
					closestHostileDistance = distance;
					sensor->closestHostileContact = sensor->contacts[i];
				}
			}
			else if (checkFac->isFriendly(fac)) {
				if ((closestFriendlyDistance.getLength() == 0) || (distance.getLength() < closestFriendlyDistance.getLength())) {
					closestFriendlyDistance = distance;
					sensor->closestFriendlyContact = sensor->contacts[i];
				}
			}
		}

		if (sensor->contacts.size() == 0) {
			sensor->closestContact = INVALID_ENTITY;
		}
	}
}