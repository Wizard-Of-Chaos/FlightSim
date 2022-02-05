#pragma once

#ifndef GAMEFUNCTIONS_H
#define GAMEFUNCTIONS_H
#include "BaseHeader.h"
#include "IrrlichtComponent.h"
#include "HealthComponent.h"
#include "InputComponent.h"
#include "PlayerComponent.h"
#include "ShipComponent.h"
#include "WeaponInfoComponent.h"

class SceneManager;

void setDefaults(SceneManager* manager);

vector3df bulletVectorToIrrlicht(btVector3 vec);
btVector3 irrlichtVectorToBullet(vector3df vec);
vector3df randomVector();
vector3df randomRotationVector();

EntityId createDefaultShip(SceneManager* manager, vector3df position);
EntityId createDefaultObstacle(SceneManager* manager, vector3df position);
EntityId createProjectileEntity(SceneManager* manager, vector3df spawnPos, vector3df direction, EntityId weaponId);
EntityId createDefaultAIShip(SceneManager* manager, vector3df position);

void destroyProjectile(SceneManager* manager, EntityId projectile);
void destroyObject(SceneManager* manager, EntityId id);

bool initializeDefaultWeapon(SceneManager* manager, EntityId shipId, int hardpoint);
bool initializeDefaultRigidBody(SceneManager* manager, EntityId objectId);
bool initializeDefaultPlayer(SceneManager* manager, EntityId shipId);
void initializeDefaultHealth(SceneManager* manager, EntityId objectId);
bool initializeDefaultHUD(SceneManager* manager, EntityId playerId);

#endif