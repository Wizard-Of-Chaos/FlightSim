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

/*
void AIUpdateSystem(f32 dt)
{
	for (auto id : SceneView<AIComponent, IrrlichtComponent, BulletRigidBodyComponent, ShipComponent>(sceneManager->scene)) 
	{
		auto AI = sceneManager->scene.get<AIComponent>(id);
		switch (AI->AIType) {
			case AI_TYPE_DEFAULT: //Right now its the only type of AI, but there will be others
				defaultAIUpdateSystem(id, dt);
				break;
			default:
				break;
		}
	}
}
*/