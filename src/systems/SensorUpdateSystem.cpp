#include "SensorUpdateSystem.h"
#include "SceneManager.h"
#include "GameController.h"

void sensorUpdateSystem(SceneManager* manager)
{
	for (EntityId id : SceneView<IrrlichtComponent, SensorComponent, FactionComponent>(manager->scene)) {
		auto irr = manager->scene.get<IrrlichtComponent>(id);
		auto sensor = manager->scene.get<SensorComponent>(id);
		auto fac = manager->scene.get<FactionComponent>(id);

		for (EntityId checkId : SceneView<FactionComponent, IrrlichtComponent>(manager->scene)) {
			if (checkId == id) continue;

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

		if (sensor->closestContact != INVALID_ENTITY) {
			auto closeIrr = manager->scene.get<IrrlichtComponent>(sensor->closestContact);
			if (closeIrr) closestContactDistance = closeIrr->node->getPosition() - irr->node->getPosition();
		}
		if (sensor->closestFriendlyContact != INVALID_ENTITY) {
			auto closeIrr = manager->scene.get<IrrlichtComponent>(sensor->closestContact);
			if (closeIrr) closestFriendlyDistance = closeIrr->node->getPosition() - irr->node->getPosition();
		}
		if (sensor->closestHostileContact != INVALID_ENTITY) {
			auto closeIrr = manager->scene.get<IrrlichtComponent>(sensor->closestContact);
			if (closeIrr) closestHostileDistance = closeIrr->node->getPosition() - irr->node->getPosition();
		}

		for (unsigned int i = 0; i < sensor->contacts.size(); ++i) {
			EntityId checkId = sensor->contacts[i];
			if (!manager->scene.entityInUse(checkId)) {
				sensor->contacts[i] = INVALID_ENTITY;
				if (sensor->closestContact == checkId) sensor->closestContact = INVALID_ENTITY;
				if (sensor->closestHostileContact == checkId) sensor->closestHostileContact = INVALID_ENTITY;
				if (sensor->closestFriendlyContact == checkId) sensor->closestFriendlyContact = INVALID_ENTITY;
				continue;
			}
			auto irrCheck = manager->scene.get<IrrlichtComponent>(checkId);

			vector3df distance = irrCheck->node->getPosition() - irr->node->getPosition();

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