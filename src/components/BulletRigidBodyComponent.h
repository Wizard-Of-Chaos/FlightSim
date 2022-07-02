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
	btRigidBody* rigidBody;
	btCollisionShape* shape;
	BulletRigidBodyComponent() : rigidBody(nullptr), shape(nullptr) {}
};

#endif
