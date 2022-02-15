#include "SceneManager.h"
#include "DefaultAIUpdateSystem.h"
#include <iostream>

void defaultAIStateCheck(SceneManager* manager, EntityId id)
{
	auto irrAI = manager->scene.get<IrrlichtComponent>(id);
	auto aiComp = manager->scene.get<AIComponent>(id);
	auto sensors = manager->scene.get<SensorComponent>(id);
	auto hp = manager->scene.get<HealthComponent>(id);

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

void defaultAIUpdateSystem(SceneManager* manager, EntityId id, f32 dt)
{
	auto ai = manager->scene.get<AIComponent>(id);
	auto sensors = manager->scene.get<SensorComponent>(id);

	ai->timeSinceLastStateCheck += dt;
	if (ai->timeSinceLastStateCheck >= ai->reactionSpeed) {
		defaultAIStateCheck(manager, id);
		ai->timeSinceLastStateCheck = 0;
	}
	
	switch (ai->state) {
	case AI_STATE_IDLE:
		defaultIdleBehavior(manager, id, dt);
		break;
	case AI_STATE_FLEE:
		defaultFleeBehavior(manager, id, sensors->closestHostileContact, dt);
		break;
	case AI_STATE_PURSUIT:
		defaultPursuitBehavior(manager, id, sensors->closestHostileContact, dt);
		break;
	default:
		break;
	}
}