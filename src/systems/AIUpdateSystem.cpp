#include "AIUpdateSystem.h"

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

		switch (ai->AIType) {
		case AI_TYPE_DEFAULT:
			defaultAIUpdateSystem(ai, irr, rbc, ship, sensors, hp, it.delta_time());
			break;
		default:
			break;
		}
	}
}
