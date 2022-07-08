#include "AIUpdateSystem.h"
#include "AIComponent.h"
#include "AITypes.h"

void AIUpdateSystem(flecs::iter it, 
	AIComponent* aic, IrrlichtComponent* irrc, BulletRigidBodyComponent* rbcs, ShipComponent* shipc, SensorComponent* sensc, HealthComponent* hpc)
{
	for (auto i : it) {
		auto ai = &aic[i];
		auto ship = &shipc[i];
		auto rbc = &rbcs[i];
		auto irr = &irrc[i];
		auto sensors = &sensc[i];
		auto hp = &hpc[i];

		f32 dt = it.delta_time();
		ai->timeSinceLastStateCheck += dt;
		if (ai->timeSinceLastStateCheck >= ai->reactionSpeed) {
			ai->aiControls->stateCheck(ai, sensors, hp);
			ai->timeSinceLastStateCheck = 0;
		}
		switch (ai->state) {
		case AI_STATE_IDLE:
			ai->aiControls->idle(ship, rbc);
			break;
		case AI_STATE_FLEE:
			ai->aiControls->flee(ship, rbc, irr, sensors->closestHostileContact);
			break;
		case AI_STATE_PURSUIT:
			ai->aiControls->pursue(ship, rbc, irr, sensors, sensors->closestHostileContact, dt);
			break;
		default:
			break;
		}
	}
		/*
		switch (ai->type) {
		case AI_TYPE_DEFAULT:
			defaultAIUpdateSystem(ai, irr, rbc, ship, sensors, hp, it.delta_time());
			break;
		case AI_TYPE_ACE:
			defaultAIUpdateSystem(ai, irr, rbc, ship, sensors, hp, it.delta_time());
			break;
		default:
			defaultAIUpdateSystem(ai, irr, rbc, ship, sensors, hp, it.delta_time());
			break;
		}
		*/
}
