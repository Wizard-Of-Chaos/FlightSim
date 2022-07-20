#pragma once

#ifndef SHIPMOVEMENTUTILS_H
#define SHIPMOVEMENTUTILS_H
#include "BaseHeader.h"
#include "ShipComponent.h"
#include "BulletRigidBodyComponent.h"
#include "IrrlichtComponent.h"
#include "ThrustComponent.h"
#include "btUtils.h"

const f32 DEGENERATE_VECTOR_LENGTH = 0.0000001f;

/*
* Holds a whole bunch of functions that are mostly used with AI to make the ship move around.
* Also includes bullet normalized vectors and the forces to move in specific directions.
*/

//Converts a Bullet quaternion to Euler angles (in degrees).
void QuaternionToEuler(const btQuaternion& TQuat, btVector3& TEuler);

//Safe normalization for a velocity that dodges degenerate cases.
btVector3 velocitySafeNormalize(btVector3& vel);

/*FORCE VECTORS*/

//Gets a force in the forward direction, modified by the ship's forward thrust.
btVector3 getForceForward(btRigidBody* body, ThrustComponent* thrust);

//Gets an afterburner force in the ship's forward direction.
btVector3 getForceBoost(btRigidBody* body, ThrustComponent* thrust);

//Gets a force in the backward direction, modified by the ship's braking thrust.
btVector3 getForceBackward(btRigidBody* body, ThrustComponent* thrust);

//Gets a force in the right direction, modified by the ship's strafing thrust.
btVector3 getForceRight(btRigidBody* body, ThrustComponent* thrust);

//Gets a force in the left direction, modified by the ship's strafing thrust.
btVector3 getForceLeft(btRigidBody* body, ThrustComponent* thrust);

//Gets a force in the up direction, modified by the ship's strafing thrust.
btVector3 getForceUp(btRigidBody* body, ThrustComponent* thrust);

//Gets a force in the down direction, modifed by the ship's strafing thrust.
btVector3 getForceDown(btRigidBody* body, ThrustComponent* thrust);

/* ROTATIONAL VECTORS */

//Gets a torque to pitch up, modified by the ship's pitch thrust.
btVector3 getTorquePitchUp(btRigidBody* body, ThrustComponent* thrust);

//Gets a torque to pitch down, modified by the ship's pitch thrust.
btVector3 getTorquePitchDown(btRigidBody* body, ThrustComponent* thrust);

//Gets a torque to yaw right, modified by the ship's yaw thrust.
btVector3 getTorqueYawRight(btRigidBody* body, ThrustComponent* thrust);

//Gets a torque to yaw left, modified by the ship's yaw thrust.
btVector3 getTorqueYawLeft(btRigidBody* body, ThrustComponent* thrust);

//Gets a torque to roll right, modified by the ship's roll thrust.
btVector3 getTorqueRollRight(btRigidBody* body, ThrustComponent* thrust);

//Gets a torque to roll left, modified by the ship's roll thrust.
btVector3 getTorqueRollLeft(btRigidBody* body, ThrustComponent* thrust);

/* MOVEMENT VECTORS */

//Returns the torque in the opposite direction of the current angular velocity, modified by the ship's rotational speed.
btVector3 getTorqueToStopAngularVelocity(btRigidBody* body, ThrustComponent* thrust);

//Returns the force in the opposite direction of the current linear velocity, modified by the ship's speed.
btVector3 getForceToStopLinearVelocity(btRigidBody* body, ThrustComponent* thrust);

//Updates the ship's moves array to pitch and yaw towards the given direction.
void turnToDirection(btRigidBody* body, ThrustComponent* thrust, btVector3 dir);

//Combines torque and opposing torque to try and smoothly turn towards the direction.
void smoothTurnToDirection(btRigidBody* body, ThrustComponent* thrust, btVector3 dir);

//Applies a torque to turn towards the given direction, then applies a force to get there. Applies a braking force when close enough.
void goToPoint(btRigidBody* body, ThrustComponent* thrust, btVector3 dest, f32 dt);

//Applies torque and force to avoid smacking into something. Returns true if actively avoiding an obstacle, and false otherwise.
bool avoidObstacles(ThrustComponent* thrust, BulletRigidBodyComponent* rbc, IrrlichtComponent* irr, f32 dt, flecs::entity target=INVALID_ENTITY);

#endif 