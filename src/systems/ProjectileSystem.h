#pragma once

#ifndef PROJECTILESYSTEM_H
#define PROJECTILESYSTEM_H

#include "BaseHeader.h"
#include "WeaponInfoComponent.h"
#include "IrrlichtComponent.h"
#include "BulletRigidBodyComponent.h"
#include "GameFunctions.h"

//Determines whether or not a projectile is out of range and deletes it accordingly. In the future,
//this will also handle the different "types" of projectile - for example, how a missile moves, how a harpoon moves, etc.
void projectileSystem(flecs::iter it, BulletRigidBodyComponent* rbcs, ProjectileInfoComponent* pic, IrrlichtComponent* irrs);


#endif