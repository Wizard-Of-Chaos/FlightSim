#include "ObjectiveSystem.h"
#include "GameController.h"
#include "GameStateController.h"

bool objectiveSystem(f32 dt)
{
	bool ret = true;
	for (auto id : SceneView<ObjectiveComponent, IrrlichtComponent>(sceneManager->scene)) {
		if (!isObjectiveCompleted(id)) {
			ret = false;
			break;
		}
	}
	return ret;
}