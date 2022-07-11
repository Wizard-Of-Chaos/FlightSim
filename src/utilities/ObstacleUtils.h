#pragma once
#ifndef OBSTACLEUTILS_H
#define OBSTACLEUTILS_H
#include "BaseHeader.h"
#include "BulletGhostComponent.h"

/*
* Creates a dynamic obstacle from the given ID. Usually called from other functions. Pulls the data out of the obstacle
* data and gives it the given scale, position, rotation, mass, and sets it up with rigid body data if applicable.
*/
flecs::entity createDynamicObstacle(u32 id, vector3df position, vector3df rotation, vector3df scale, f32 mass);
//Creates a static obstacle. Effectively the same as a dynamic obstacle, except that with a mass of 0, the obstacle doesn't move.
flecs::entity createStaticObstacle(u32 id, vector3df position, vector3df rotation, vector3df scale);

//Creates a gas cloud at the given position with the given scale. Includes health and irrlicht components. Returns the ID.
flecs::entity createGasCloud(vector3df position, vector3df scale);
//Creates an asteroid at the given position. Includes health, irrlicht, and rigid body components. Returns the ID.
flecs::entity createAsteroid(vector3df position, vector3df rotation, vector3df scale, f32 mass);
//Creates an explosive asteroid at the given position. Returns the ID.
flecs::entity createExplosiveAsteroid(vector3df position, vector3df rotation, vector3df scale, f32 mass);
//Creates some debris at the given position. Returns the ID.
flecs::entity createDebris(vector3df position, vector3df rotation, vector3df scale, f32 mass);

//Callback for when a gas cloud blows up. Stand back.
void deathExplosion(flecs::entity id);
#endif 