#include "SceneManager.h"
#include "DefaultAIUpdateSystem.h"
#include <iostream>

void updateContacts(SceneManager* manager, EntityId id)
{
	auto irrAI = manager->scene.get<IrrlichtComponent>(id);
	auto aiComp = manager->scene.get<AIComponent>(id);
	auto facAI = manager->scene.get<FactionComponent>(id);

	for (EntityId checkId : SceneView<FactionComponent, IrrlichtComponent>(manager->scene)) {
		if (checkId == id) continue;

		auto irrCheck = manager->scene.get<IrrlichtComponent>(checkId);
		vector3df distance = irrCheck->node->getPosition() - irrAI->node->getPosition();
		if (distance.getLength() <= aiComp->detectionRadius) {
			if (aiComp->contacts.binary_search(checkId) == -1) {
				aiComp->contacts.push_back(checkId);
			}
		}
	}
	vector3df closestContactDistance(0, 0, 0);
	vector3df closestHostileDistance(0, 0, 0);
	vector3df closestFriendlyDistance(0, 0, 0);

	if (aiComp->closestContact != INVALID_ENTITY) {
		auto closeIrr = manager->scene.get<IrrlichtComponent>(aiComp->closestContact);
		if (closeIrr) closestContactDistance = closeIrr->node->getPosition() - irrAI->node->getPosition();
	}
	if (aiComp->closestFriendlyContact != INVALID_ENTITY) {
		auto closeIrr = manager->scene.get<IrrlichtComponent>(aiComp->closestContact);
		if(closeIrr) closestFriendlyDistance = closeIrr->node->getPosition() - irrAI->node->getPosition();
	}
	if (aiComp->closestHostileContact != INVALID_ENTITY) {
		auto closeIrr = manager->scene.get<IrrlichtComponent>(aiComp->closestContact);
		if(closeIrr) closestHostileDistance = closeIrr->node->getPosition() - irrAI->node->getPosition();
	}

	for (unsigned int i = 0; i < aiComp->contacts.size(); ++i) {
		EntityId checkId = aiComp->contacts[i];
		if (!manager->scene.entityInUse(checkId)) {
			aiComp->contacts[i] = INVALID_ENTITY;
			if (aiComp->closestContact == checkId) aiComp->closestContact = INVALID_ENTITY;
			if (aiComp->closestHostileContact == checkId) aiComp->closestHostileContact = INVALID_ENTITY;
			if (aiComp->closestFriendlyContact == checkId) aiComp->closestFriendlyContact = INVALID_ENTITY;
			continue;
		}
		auto irrCheck = manager->scene.get<IrrlichtComponent>(checkId);

		vector3df distance = irrCheck->node->getPosition() - irrAI->node->getPosition();

		if (distance.getLength() > aiComp->detectionRadius) {
			if (aiComp->closestContact == checkId) {
				aiComp->closestContact = INVALID_ENTITY;
				closestContactDistance = vector3df(0, 0, 0);
			}
			if (aiComp->closestFriendlyContact == checkId) {
				aiComp->closestFriendlyContact = INVALID_ENTITY;
				closestFriendlyDistance = vector3df(0, 0, 0);
			}
			if (aiComp->closestHostileContact == checkId) {
				aiComp->closestHostileContact = INVALID_ENTITY;
				closestHostileDistance = vector3df(0, 0, 0);
			}

			aiComp->contacts.erase(i);
			continue;
		}

		if ((distance.getLength() < closestContactDistance.getLength()) || (closestContactDistance.getLength() == 0)) {
			aiComp->closestContact = aiComp->contacts[i];
			closestContactDistance = distance;
		}

		auto checkFac = manager->scene.get<FactionComponent>(checkId);
		if (!checkFac) continue;

		if (checkFac->isHostile(facAI)) {
			if ((closestHostileDistance.getLength() == 0) || (distance.getLength() < closestHostileDistance.getLength())) {
				closestHostileDistance = distance;
				aiComp->closestHostileContact = aiComp->contacts[i];
			}
		}
		else if (checkFac->isFriendly(facAI)) {
			if ((closestFriendlyDistance.getLength() == 0) || (distance.getLength() < closestFriendlyDistance.getLength())) {
				closestFriendlyDistance = distance;
				aiComp->closestFriendlyContact = aiComp->contacts[i];
			}
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
	auto hp = manager->scene.get<HealthComponent>(id);

	if (aiComp->closestHostileContact == INVALID_ENTITY) {
		aiComp->state = AI_STATE_IDLE;
		return;
	}
	else if (hp->health <= (hp->maxHealth * .25f)) {
		//there's a hostile, but I'm low on health!
		aiComp->state = AI_STATE_FLEE; //aaaaieeeee!
		return;
	}
	//there's a hostile and I can take him!
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
		defaultFleeBehavior(manager, id, ai->closestHostileContact, dt);
		break;
	case AI_STATE_PURSUIT:
		defaultPursuitBehavior(manager, id, ai->closestHostileContact, dt);
		break;
	default:
		break;
	}
}