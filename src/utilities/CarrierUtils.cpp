#include "CarrierUtils.h"
#include "GameController.h"
#include "GameStateController.h"
#include "SceneManager.h"
#include "GuiController.h"

void initializeCarrier(EntityId id, f32 spawnRate, u32 reserveShips, vector3df scale)
{
	auto carr = sceneManager->scene.assign<CarrierComponent>(id);
	carr->reserveShips = reserveShips;
	carr->spawnRate = spawnRate;
	carr->scale = scale;
}

EntityId createCarrier(bool human, vector3df pos, vector3df rot, vector3df scale)
{
	EntityId carrier = createDynamicObstacle(3, pos, rot, scale, scale.X); 
	return carrier;
}