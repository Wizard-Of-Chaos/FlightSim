#pragma once
#ifndef OBSTACLEUTILS_H
#define OBSTACLEUTILS_H
#include "BaseHeader.h"
#include "BulletGhostComponent.h"
#include "ECS.h"

class GameController;
class SceneManager;

//Creates a gas cloud at the given position with the given scale. Includes health and irrlicht components. Returns the ID.
EntityId createGasCloud(SceneManager* manager, vector3df position, vector3df scale);
//Creates an asteroid at the given position. Includes health, irrlicht, and rigid body components. Returns the ID.
EntityId createAsteroid(SceneManager* manager, vector3df position, vector3df rotation, vector3df scale, f32 mass);
#endif 