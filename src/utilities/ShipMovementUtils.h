#pragma once

#ifndef SHIPMOVEMENTUTILS_H
#define SHIPMOVEMENTUTILS_H
#include "BaseHeader.h"
#include "ECS.h"
#include "ShipComponent.h"
#include "BulletRigidBodyComponent.h"
#include "IrrlichtComponent.h"

class SceneManager;
class GameController;

/*
* Holds a whole bunch of functions that are mostly used with AI to make the ship move around.
* Also includes bullet normalized vectors and the forces to move in specific directions.
*/

//Converts a Bullet quaternion to Euler angles (in degrees).
void QuaternionToEuler(const btQuaternion& TQuat, btVector3& TEuler);

//Safe normalization for a velocity that dodges degenerate cases.
btVector3 velocitySafeNormalize(btVector3& vel);

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

//Gets a force in the forward direction, modified by the ship's forward thrust.
btVector3 getForceForward(btRigidBody* body, ShipComponent* ship);

//Gets a force in the backward direction, modified by the ship's braking thrust.
btVector3 getForceBackward(btRigidBody* body, ShipComponent* ship);

//Gets a force in the right direction, modified by the ship's strafing thrust.
btVector3 getForceRight(btRigidBody* body, ShipComponent* ship);

//Gets a force in the left direction, modified by the ship's strafing thrust.
btVector3 getForceLeft(btRigidBody* body, ShipComponent* ship);

//Gets a force in the up direction, modified by the ship's strafing thrust.
btVector3 getForceUp(btRigidBody* body, ShipComponent* ship);

//Gets a force in the down direction, modifed by the ship's strafing thrust.
btVector3 getForceDown(btRigidBody* body, ShipComponent* ship);

/* ROTATIONAL VECTORS */

//Gets a torque to pitch up, modified by the ship's pitch thrust.
btVector3 getTorquePitchUp(btRigidBody* body, ShipComponent* ship);

//Gets a torque to pitch down, modified by the ship's pitch thrust.
btVector3 getTorquePitchDown(btRigidBody* body, ShipComponent* ship);

//Gets a torque to yaw right, modified by the ship's yaw thrust.
btVector3 getTorqueYawRight(btRigidBody* body, ShipComponent* ship);

//Gets a torque to yaw left, modified by the ship's yaw thrust.
btVector3 getTorqueYawLeft(btRigidBody* body, ShipComponent* ship);

//Gets a torque to roll right, modified by the ship's roll thrust.
btVector3 getTorqueRollRight(btRigidBody* body, ShipComponent* ship);

//Gets a torque to roll left, modified by the ship's roll thrust.
btVector3 getTorqueRollLeft(btRigidBody* body, ShipComponent* ship);

/* MOVEMENT VECTORS */

//Returns the torque in the opposite direction of the current angular velocity, modified by the ship's rotational speed.
btVector3 getTorqueToStopAngularVelocity(btRigidBody* body, ShipComponent* ship);

//Returns the force in the opposite direction of the current linear velocity, modified by the ship's speed.
btVector3 getForceToStopLinearVelocity(btRigidBody* body, ShipComponent* ship);

//Updates the ship's moves array to pitch and yaw towards the given direction.
void turnToDirection(btRigidBody* body, ShipComponent* ship, btVector3 dir);

//Combines torque and opposing torque to try and smoothly turn towards the direction.
void smoothTurnToDirection(btRigidBody* body, ShipComponent* ship, btVector3 dir, f32 dt);

//Applies a torque to turn towards the given direction, then applies a force to get there. Applies a braking force when close enough.
void goToPoint(btRigidBody* body, ShipComponent* ship, btVector3 dest, f32 dt);

//Applies torque and force to avoid smacking into something. Returns true if actively avoiding an obstacle, and false otherwise.
bool avoidObstacles(SceneManager* manager, EntityId id, f32 dt, EntityId target=INVALID_ENTITY);

#endif 