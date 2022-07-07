#include "HealthAndShieldSystems.h"

#include "GameStateController.h"
#include "GameController.h"
#include <iostream>

void healthSystem(flecs::iter it, HealthComponent* hc)
{
	for (auto i : it)
	{
		auto healthComponent = &hc[i];
		auto e = it.entity(i);
		if (healthComponent->health <= 0)
		{
			auto irrComp = e.get<IrrlichtComponent>();
			auto playerComp = e.get<PlayerComponent>();

			if (playerComp)
			{
				gameController->isPlayerAlive = false;
				gameController->clearPlayerHUD();
			}
			if (gameController->hasDeathCallback(e))
			{
				gameController->deathCallbacks[e](e);
			}
			else if (irrComp)
			{
				auto pos = irrComp->node->getAbsolutePosition();
				auto scale = irrComp->node->getScale();
				auto avgScale = (scale.X + scale.Y + scale.Z) / 6.f;
				auto rad = irrComp->node->getBoundingBox().getExtent().getLength() * avgScale;
				explode(pos, 3.f, avgScale, rad, 50.f, 800.f);
			}
			destroyObject(e);
			if (!gameController->isPlayerAlive)
			{
				stateController->setState(GAME_FINISHED);
			}
		}
	}
}

void shieldSystem(flecs::iter it, ShieldComponent* sc)
{
	for (auto i : it)
	{
		auto shieldComponent = &sc[i];
		shieldComponent->timeSinceLastHit += it.delta_system_time();

		if (shieldComponent->timeSinceLastHit >= shieldComponent->rechargeDelay)
		{
			shieldComponent->timeSinceLastHit = shieldComponent->rechargeDelay + .5f;
			if (shieldComponent->shields < shieldComponent->maxShields)
			{
				shieldComponent->shields += shieldComponent->rechargeRate * it.delta_time();
				if (shieldComponent->shields > shieldComponent->maxShields) 
				{
					shieldComponent->shields = shieldComponent->maxShields;
				}
			}
		}
	}
}