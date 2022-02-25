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
			auto irr = manager->scene.get<IrrlichtComponent>(id);
			auto player = manager->scene.get<PlayerComponent>(id);
			if (player) playerDead = true;

			if (irr) {
				explode(manager, irr->node->getPosition(), 1.f);
			}
			destroyObject(manager, id); //get rid of the object first, THEN change state
			if (playerDead) {
				manager->controller->stateController->setState(GAME_DEAD);
			}
		}
	}
}