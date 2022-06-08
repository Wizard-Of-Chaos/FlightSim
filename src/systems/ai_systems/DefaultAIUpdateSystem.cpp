#include "DefaultAIUpdateSystem.h"
#include <iostream>

void defaultAIStateCheck(flecs::entity id)
{
	if (!id.is_alive()) return;
	if (!id.has<AIComponent>() || !id.has<SensorComponent>() || !id.has<HealthComponent>()) return;

	auto aiComp = id.get_mut<AIComponent>();
	auto sensors = id.get<SensorComponent>();
	auto hp = id.get<HealthComponent>();

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

void defaultAIUpdateSystem(flecs::entity id, f32 dt)
{
	if (!id.is_alive()) return;
	if (!id.has<AIComponent>() || !id.has<SensorComponent>()) return;
	auto ai = id.get_mut<AIComponent>();
	auto sensors = id.get_mut<SensorComponent>();

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