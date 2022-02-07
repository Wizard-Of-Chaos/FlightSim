#pragma once

#ifndef SHIPMOVEMENTUTILS_H
#define SHIPMOVEMENTUTILS_H
#include "BaseHeader.h"
#include "ECS.h"
#include "ShipComponent.h"
#include "BulletRigidBodyComponent.h"
#include "IrrlichtComponent.h"

//Converts a Bullet quaternion to Euler angles (in degrees).
void QuaternionToEuler(const btQuaternion& TQuat, btVector3& TEuler);

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

/*FORCE VECTORS*/

//Gets a force in the forward direction, modified by the ship's speed.
btVector3 getForceForward(btRigidBody* body, ShipComponent* ship);

//Gets a force in the backward direction, modified by the ship's speed.
btVector3 getForceBackward(btRigidBody* body, ShipComponent* ship);

//Gets a force in the right direction, modified by the ship's speed.
btVector3 getForceRight(btRigidBody* body, ShipComponent* ship);

//Gets a force in the left direction, modified by the ship's speed.
btVector3 getForceLeft(btRigidBody* body, ShipComponent* ship);

//Gets a force in the up direction, modified by the ship's speed.
btVector3 getForceUp(btRigidBody* body, ShipComponent* ship);

//Gets a force in the down direction, modifed by the ship's speed.
btVector3 getForceDown(btRigidBody* body, ShipComponent* ship);

/* ROTATIONAL VECTORS */

//Gets a torque to pitch up, modified by the ship's rotational speed.
btVector3 getTorquePitchUp(btRigidBody* body, ShipComponent* ship);

//Gets a torque to pitch down, modified by the ship's rotational speed.
btVector3 getTorquePitchDown(btRigidBody* body, ShipComponent* ship);

//Gets a torque to yaw right, modified by the ship's rotational speed.
btVector3 getTorqueYawRight(btRigidBody* body, ShipComponent* ship);

//Gets a torque to yaw left, modified by the ship's rotational speed.
btVector3 getTorqueYawLeft(btRigidBody* body, ShipComponent* ship);

//Gets a torque to roll right, modified by the ship's rotational speed.
btVector3 getTorqueRollRight(btRigidBody* body, ShipComponent* ship);

//Gets a torque to roll left, modified by the ship's rotational speed.
btVector3 getTorqueRollLeft(btRigidBody* body, ShipComponent* ship);

/* MOVEMENT VECTORS */

//Returns torque on an axis, modified by the ship's rotational speed. Use to roll, pitch, and yaw.
btVector3 getTorqueOnAxis(ShipComponent* ship, btVector3 axis);

//Returns the torque in the opposite direction of the current angular velocity, modified by the ship's rotational speed.
btVector3 getTorqueToStopAngularVelocity(btRigidBody* body, ShipComponent* ship);

//Returns the force in the opposite direction of the current linear velocity, modified by the ship's speed.
btVector3 getForceToStopLinearVelocity(btRigidBody* body, ShipComponent* ship);

//Returns the force for the ship to move in the direction of the given vector.
btVector3 getForceToDirection(ShipComponent* ship, btVector3 dir);

//Returns the torque to kick the ship in the direction of the given vector.
btVector3 getTorqueToDirection(btRigidBody* body, ShipComponent* ship, btVector3 dir);

//Returns the torque to go opposite of the direction vector.
btVector3 getTorqueOpposingDirection(btRigidBody* body, ShipComponent* ship, btVector3 dir);

//Combines torque and opposing torque to try and smoothly turn towards the direction.
void smoothTurnToDirection(btRigidBody* body, ShipComponent* ship, btVector3 dir, f32 dt);

#endif 