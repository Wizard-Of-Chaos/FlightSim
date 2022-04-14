#pragma once
#ifndef OBSTACLEUTILS_H
#define OBSTACLEUTILS_H
#include "BaseHeader.h"
#include "ECS.h"

class GameController;
class SceneManager;

EntityId createGasCloud(SceneManager* manager, vector3df position, vector3df scale);

#endif 