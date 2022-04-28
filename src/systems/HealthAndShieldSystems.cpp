#include "HealthAndShieldSystems.h"
#include "SceneManager.h"
#include "GameStateController.h"
#include "GameController.h"
#include <iostream>

void updateHealthSystem()
{
	for (auto id : SceneView<HealthComponent>(sceneManager->scene)) {
		auto hpComp = sceneManager->scene.get<HealthComponent>(id);
		if (hpComp->health <= 0) {
			auto irr = sceneManager->scene.get<IrrlichtComponent>(id);
			auto player = sceneManager->scene.get<PlayerComponent>(id);
			if (player) {
				gameController->isPlayerAlive = false;
				gameController->clearPlayerHUD();
			}
			if (gameController->hasDeathCallback(id)) {
				gameController->deathCallbacks[id](id);
			}
			else if (irr) {
				vector3df pos = irr->node->getAbsolutePosition();
				vector3df scale = irr->node->getScale();
				f32 avgscale = (scale.X + scale.Y + scale.Z) / 6.f;
				f32 rad = irr->node->getBoundingBox().getExtent().getLength() * avgscale;
				explode(pos, 3.f, avgscale, rad, 50.f, 800.f);
			}
			destroyObject(id); //get rid of the object first, THEN change state
			if (!gameController->isPlayerAlive) {
				stateController->setState(GAME_FINISHED);
			}
		}
	}
}

void updateShieldSystem(f32 dt)
{
	for (auto id : SceneView<ShieldComponent>(sceneManager->scene)) {
		auto shieldComp = sceneManager->scene.get<ShieldComponent>(id);
		shieldComp->timeSinceLastHit += dt;
		if (shieldComp->timeSinceLastHit >= shieldComp->rechargeDelay) {
			shieldComp->timeSinceLastHit = shieldComp->rechargeDelay + .5f;

			if (shieldComp->shields < shieldComp->maxShields) {
				shieldComp->shields += shieldComp->rechargeRate * dt;
				if (shieldComp->shields > shieldComp->maxShields) {
					shieldComp->shields = shieldComp->maxShields;
				}
			}
		}
	}
}