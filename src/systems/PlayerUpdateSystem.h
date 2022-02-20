#pragma once

#ifndef PLAYERUPDATESYSTEM_H
#define PLAYERUPDATESYSTEM_H
#include "BaseHeader.h"

#include "ECS.h"
#include "IrrlichtComponent.h"
#include "PlayerComponent.h"
#include "BulletRigidBodyComponent.h"
#include "IrrlichtUtils.h"

#include "HUDHeader.h"

class SceneManager;
class GameController;

void playerUpdateSystem(SceneManager* manager, Scene& scene, f32 dt);
void cameraUpdate(PlayerComponent* player, ISceneNode* playerShip, btRigidBody* body);
void hudUpdate(SceneManager* manager, PlayerComponent* player, EntityId playerId);
#endif
