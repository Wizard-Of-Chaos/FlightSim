#include "AIUpdateSystem.h"
#include "SceneManager.h"

void AIUpdateSystem(f32 dt)
{
	for (auto id : SceneView<AIComponent, IrrlichtComponent, BulletRigidBodyComponent, ShipComponent>(sceneManager->scene)) {
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
