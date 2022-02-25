#pragma once

#ifndef PROJECTILESYSTEM_H
#define PROJECTILESYSTEM_H

#include "BaseHeader.h"
#include "ECS.h"
#include "WeaponInfoComponent.h"
#include "IrrlichtComponent.h"
#include "BulletRigidBodyComponent.h"
#include "GameFunctions.h"

class GameController;
class SceneManager;

//Determines whether or not a projectile is out of range and deletes it accordingly. In the future,
//this will also handle the different "types" of projectile - for example, how a missile moves, how a harpoon moves, etc.
void projectileRangeSystem(SceneManager* manager);

#endif