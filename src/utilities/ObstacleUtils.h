#pragma once
#ifndef OBSTACLEUTILS_H
#define OBSTACLEUTILS_H
#include "BaseHeader.h"
#include "BulletGhostComponent.h"

flecs::entity createDynamicObstacle(u32 id, vector3df position, vector3df rotation, vector3df scale, f32 mass);
flecs::entity createStaticObstacle(u32 id, vector3df position, vector3df rotation, vector3df scale);

//Creates a gas cloud at the given position with the given scale. Includes health and irrlicht components. Returns the ID.
flecs::entity createGasCloud(vector3df position, vector3df scale);
//Creates an asteroid at the given position. Includes health, irrlicht, and rigid body components. Returns the ID.
flecs::entity createAsteroid(vector3df position, vector3df rotation, vector3df scale, f32 mass);
flecs::entity createDebris(vector3df position, vector3df rotation, vector3df scale, f32 mass);

//Callback for when a gas cloud blows up. Stand back.
void gasDeathExplosion(flecs::entity id);
#endif 