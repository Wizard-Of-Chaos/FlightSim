#include "SceneManager.h"
#include "DefaultAIUpdateSystem.h"
#include <iostream>

void updateContacts(SceneManager* manager, EntityId id)
{
	auto irrAI = manager->scene.get<IrrlichtComponent>(id);
	auto aiComp = manager->scene.get<AIComponent>(id);
	for (EntityId checkId : SceneView<ShipComponent, IrrlichtComponent>(manager->scene)) {
		if (manager->scene.get<AIComponent>(checkId)) continue;

		auto irrCheck = manager->scene.get<IrrlichtComponent>(checkId);
		vector3df distance = irrCheck->node->getPosition() - irrAI->node->getPosition();
		if (distance.getLength() <= aiComp->detectionRadius) {
			if (aiComp->contacts.binary_search(checkId) == -1) {
				aiComp->contacts.push_back(checkId);
			}
		}
	}
	vector3df closestContactDistance(0, 0, 0);

	if (aiComp->closestContact != INVALID_ENTITY) {
		auto closeIrr = manager->scene.get<IrrlichtComponent>(aiComp->closestContact);
		closestContactDistance = closeIrr->node->getPosition() - irrAI->node->getPosition();
	}

	for (unsigned int i = 0; i < aiComp->contacts.size(); ++i) {
		EntityId checkId = aiComp->contacts[i];
		auto irrCheck = manager->scene.get<IrrlichtComponent>(checkId);
		vector3df distance = irrCheck->node->getPosition() - irrAI->node->getPosition();

		if (distance.getLength() > aiComp->detectionRadius) {
			aiComp->contacts.erase(i);
			continue;
		}
		if (closestContactDistance.getLength() == 0) {
			closestContactDistance = distance;
			aiComp->closestContact = aiComp->contacts[i];
		}

		if (distance.getLength() < closestContactDistance.getLength()) {
			aiComp->closestContact = aiComp->contacts[i];
			closestContactDistance = distance;
		}
	}
	if (aiComp->contacts.size() == 0) {
		aiComp->closestContact = INVALID_ENTITY;
	}
}

void defaultAIStateCheck(SceneManager* manager, EntityId id)
{
	auto irrAI = manager->scene.get<IrrlichtComponent>(id);
	auto aiComp = manager->scene.get<AIComponent>(id);

	if (aiComp->closestContact == INVALID_ENTITY) {
		aiComp->state = AI_STATE_IDLE;
		return;
	}

	//there IS a contact!
	aiComp->state = AI_STATE_PURSUIT;
	//whoop its ass!
}

void defaultAIUpdateSystem(SceneManager* manager, EntityId id, f32 dt)
{
	auto ai = manager->scene.get<AIComponent>(id);
	ai->timeSinceLastStateCheck += dt;
	if (ai->timeSinceLastStateCheck >= ai->reactionSpeed) {
		updateContacts(manager, id);
		defaultAIStateCheck(manager, id);
		ai->timeSinceLastStateCheck = 0;
	}
	
	switch (ai->state) {
	case AI_STATE_IDLE:
		defaultIdleBehavior(manager, id, dt);
		break;
	case AI_STATE_FLEE:
		defaultFleeBehavior(manager, id, dt);
		break;
	case AI_STATE_PURSUIT:
		defaultPursuitBehavior(manager, id, dt);
		break;
	default:
		break;
	}
}