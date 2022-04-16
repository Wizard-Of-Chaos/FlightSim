#include "SceneManager.h"
#include "DefaultAIUpdateSystem.h"
#include <iostream>

void defaultAIStateCheck(EntityId id)
{
	auto irrAI = sceneManager->scene.get<IrrlichtComponent>(id);
	auto aiComp = sceneManager->scene.get<AIComponent>(id);
	auto sensors = sceneManager->scene.get<SensorComponent>(id);
	auto hp = sceneManager->scene.get<HealthComponent>(id);

	if (sensors->closestHostileContact == INVALID_ENTITY) {
		aiComp->state = AI_STATE_IDLE;
		return;
	}
	else if (hp->health <= (hp->maxHealth * aiComp->damageTolerance)) {
		//there's a hostile, but I'm low on health!
		aiComp->state = AI_STATE_FLEE; //aaaaieeeee!
		return;
	}
	//there's a hostile and I can take him!
	aiComp->state = AI_STATE_PURSUIT;
	//whoop its ass!
}

void defaultAIUpdateSystem(EntityId id, f32 dt)
{
	auto ai = sceneManager->scene.get<AIComponent>(id);
	auto sensors = sceneManager->scene.get<SensorComponent>(id);

	ai->timeSinceLastStateCheck += dt;
	if (ai->timeSinceLastStateCheck >= ai->reactionSpeed) {
		defaultAIStateCheck(id);
		ai->timeSinceLastStateCheck = 0;
	}
	
	switch (ai->state) {
	case AI_STATE_IDLE:
		defaultIdleBehavior(id, dt);
		break;
	case AI_STATE_FLEE:
		defaultFleeBehavior(id, sensors->closestHostileContact, dt);
		break;
	case AI_STATE_PURSUIT:
		defaultPursuitBehavior(id, sensors->closestHostileContact, dt);
		break;
	default:
		break;
	}
}