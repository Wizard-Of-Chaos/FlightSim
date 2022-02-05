#include "AIUpdateSystem.h"
#include "SceneManager.h"

void AIUpdateSystem(SceneManager* manager, f32 dt)
{
	for (auto id : SceneView<AIComponent, IrrlichtComponent, BulletRigidBodyComponent, ShipComponent>(manager->scene)) {
		auto AI = manager->scene.get<AIComponent>(id);
		switch (AI->AIType) {
			case AI_TYPE_DEFAULT:
				defaultAIUpdateSystem(manager, id, dt);
				break;
			default:
				break;
		}
	}
}
