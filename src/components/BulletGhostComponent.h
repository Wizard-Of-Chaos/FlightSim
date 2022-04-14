#pragma once
#ifndef BULLETGHOSTCOMPONENT_H
#define BULLETGHOSTCOMPONENT_H
#include "BaseHeader.h"
#include "ECS.h"

struct BulletGhostComponent
{
	btGhostObject ghost = btGhostObject();
	btSphereShape shape = btSphereShape(0.f);
};

#endif 