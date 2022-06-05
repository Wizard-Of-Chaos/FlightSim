#include "ObjectiveSystem.h"
#include "GameController.h"
#include "GameStateController.h"

void objectiveSystem(flecs::iter it, ObjectiveComponent* objc)
{
	bool ret = true;
	for (auto i : it) {
		auto obj = &objc[i];
		if (!isObjectiveCompleted(it.entity(i))) {
			ret = false;
			break;
		}
	}
	if (ret) {
		stateController->setState(GAME_FINISHED); //if all objectives have been completed, we're done
	}
}

/*
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
*/