#include "DefaultAIUpdateSystem.h"
#include <iostream>

//Checks as often as the AI component's reaction speed will allow it to. Updates state based on its environment.
void defaultAIStateCheck(AIComponent* aiComp, SensorComponent* sensors, HealthComponent* hp)
{
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

void defaultAIUpdateSystem(
	AIComponent* ai, IrrlichtComponent* irr, BulletRigidBodyComponent* rbc, ShipComponent* ship, SensorComponent* sensors, HealthComponent* hp,
	f32 dt)
{
	ai->timeSinceLastStateCheck += dt;
	if (ai->timeSinceLastStateCheck >= ai->reactionSpeed) {
		defaultAIStateCheck(ai, sensors, hp);
		ai->timeSinceLastStateCheck = 0;
	}
	
	switch (ai->state) {
	case AI_STATE_IDLE:
		defaultIdleBehavior(ship, rbc, dt);
		break;
	case AI_STATE_FLEE:
		defaultFleeBehavior(irr, rbc, ship, sensors->closestHostileContact, dt);
		break;
	case AI_STATE_PURSUIT:
		defaultPursuitBehavior(sensors, rbc, ship, irr, sensors->closestHostileContact, dt);
		break;
	default:
		break;
	}
}