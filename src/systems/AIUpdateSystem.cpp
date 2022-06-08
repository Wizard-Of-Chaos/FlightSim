#include "AIUpdateSystem.h"

void AIUpdateSystem(flecs::iter it, AIComponent* aic, IrrlichtComponent* irrc)
{
	for (auto i : it) {
		auto ent = it.entity(i);
		auto ai = ent.get<AIComponent>();
		switch (ai->AIType) {
		case AI_TYPE_DEFAULT:
			defaultAIUpdateSystem(ent, it.delta_time());
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