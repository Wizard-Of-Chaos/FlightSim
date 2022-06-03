#pragma once
#ifndef BULLETGHOSTCOMPONENT_H
#define BULLETGHOSTCOMPONENT_H
#include "BaseHeader.h"

/*
* Bullet ghost components are for things you can shoot at, but can't collide with.
* An example is a gas cloud; the player can fly through it but can still target it to blow it up.
* The shape is assumed to be a sphere shape (might change depending on what else we add).
*/
struct BulletGhostComponent
{
	btGhostObject ghost = btGhostObject();
	btSphereShape shape = btSphereShape(0.f);
};

#endif 