#pragma once

#ifndef HEADER_H
#define HEADER_H
#include "BaseHeader.h"

#include "ECS.h"
#include "IrrlichtComponent.h"
#include "PlayerComponent.h"
#include "BulletRigidBodyComponent.h"
#include "IrrlichtUtils.h"

class SceneManager;
class GameController;

void playerUpdateSystem(SceneManager* manager, Scene& scene, f32 dt);
void cameraUpdate(PlayerComponent* player, ISceneNode* playerShip, btRigidBody* body);
void hudUpdate(SceneManager* manager, PlayerComponent* player, ISceneNode* playerShip, InputComponent* input);
#endif
