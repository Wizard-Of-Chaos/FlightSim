#include "Scenario.h"
#include "GameController.h"
#include "GameStateController.h"

bool Scenario::detected() 
{
	u32 roll = std::rand() % 100;
	if (roll < detectionChance) return true;
	return false;
}

bool Scenario::completed()
{
	for (u32 i = 0; i < objectiveCount; ++i) {
		if (objectives[i] == INVALID_ENTITY || sceneManager->scene.entityInUse(objectives[i])) {
			objectives[i] = INVALID_ENTITY;
			--activeObjectiveCount;
		}
	}
	if(activeObjectiveCount == 0) return true;
	return false;
}