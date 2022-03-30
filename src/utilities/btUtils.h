#pragma once
#ifndef BTUTILS_H
#define BTUTILS_H
#include "BaseHeader.h"
#include "ECS.h"

//Convenience function to get the ID from a bullet collision object.
EntityId getIdFromBt(btCollisionObject* object);

/* DIRECTIONAL VECTORS */

//Forward directional vector for a rigid body.
btVector3 getRigidBodyForward(btRigidBody* body);

//Backward directional vector for a rigid body.
btVector3 getRigidBodyBackward(btRigidBody* body);

//Right directional vector for a rigid body.
btVector3 getRigidBodyRight(btRigidBody* body);

//Left directional vector for a rigid body.
btVector3 getRigidBodyLeft(btRigidBody* body);

//Up directional vector for a rigid body.
btVector3 getRigidBodyUp(btRigidBody* body);

//Down directional vector for a rigid body.
btVector3 getRigidBodyDown(btRigidBody* body);

#endif 