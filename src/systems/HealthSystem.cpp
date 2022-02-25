#include "HealthSystem.h"
#include "SceneManager.h"
#include "GameStateController.h"
#include "GameController.h"
#include <iostream>

void updateHealthSystem(SceneManager* manager)
{
	for (auto id : SceneView<HealthComponent>(manager->scene)) {
		auto hpComp = manager->scene.get<HealthComponent>(id);
		if (hpComp->health <= 0) {
			bool playerDead = false;
			auto player = manager->scene.get<PlayerComponent>(id);
			if (player) playerDead = true;
			destroyObject(manager, id); //get rid of the object first, THEN change state
			if (playerDead) {
				manager->controller->stateController->setState(GAME_DEAD);
			}
		}
	}
}