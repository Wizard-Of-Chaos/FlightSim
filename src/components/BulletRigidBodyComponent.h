#pragma once
#ifndef BULLETRIGIDBODYCOMPONENT_H
#define BULLETRIGIDBODYCOMPONENT_H
#include "BaseHeader.h"
#include "IrrlichtComponent.h"
#include "GameFunctions.h"

//This is just a wrapper around the rigid body so that it can be
//represented by a component in the Entity-Component system.
struct BulletRigidBodyComponent
{
	btRigidBody rigidBody = btRigidBody(0,0,0,btVector3(0,0,0));
	btConvexHullShape shape;
	btSphereShape sphere = btSphereShape(.5f); //used for projectiles
};

#endif
