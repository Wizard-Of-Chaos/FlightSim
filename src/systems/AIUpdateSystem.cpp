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

		if (!ai) std::cout << "NO AI\n";
		if (!sensors) std::cout << "NO SENSORS\n";
		if (!hp) std::cout << "NO HP\n";

		f32 dt = it.delta_time();
		ai->timeSinceLastStateCheck += dt;
		if (ai->timeSinceLastStateCheck >= ai->reactionSpeed) {
			ai->aiControls->stateCheck(ai, sensors, hp);
			ai->timeSinceLastStateCheck = 0;
		}
		switch (ai->state) {
			case AI_STATE_IDLE:
				ai->aiControls->formOnWing(ai, ship, rbc, dt);
				break;
			case AI_STATE_FLEE:
				ai->aiControls->flee(ship, rbc, irr, sensors->closestHostileContact);
				break;
			case AI_STATE_PURSUIT:
				ai->aiControls->pursueOnWing(ai, ship, rbc, irr, sensors, sensors->closestHostileContact, dt);
				break;
			default:
				break;
		}
	}
}
