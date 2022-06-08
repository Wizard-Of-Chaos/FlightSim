#pragma once

#ifndef PLAYERUPDATESYSTEM_H
#define PLAYERUPDATESYSTEM_H
#include "BaseHeader.h"
#include "IrrlichtComponent.h"
#include "PlayerComponent.h"
#include "BulletRigidBodyComponent.h"
#include "IrrlichtUtils.h"
#include "SensorComponent.h"

#include "HUDHeader.h"

//Calls the update on the player components in the scene. Does things like move the HUD around and rotates the camera.
//It also moves the audio "listener" around to be where the camera currently is.
void playerUpdateSystem(flecs::iter it, IrrlichtComponent* irrc, PlayerComponent* plyc, BulletRigidBodyComponent* rbcs, SensorComponent* snsc);
//Updates the camera. This is done by having an empty scene node maintain the exact position of the player. The empty node
//(the "target" node in playerComponent) does not match the rotation exactly. Instead, it slerps between its current rotation
//and the player's actual rotation, so the camera is able to to actually rotate. This gives the effect of smooth turning as well as
//visibly showing when you're moving or not. Also updates with velocity.
void cameraUpdate(PlayerComponent* player, ISceneNode* playerShip, btRigidBody* body);
//Calls all HUD update functions currently held by the player's HUD - things like contacts, the active selection, etc.
void hudUpdate(PlayerComponent* player, flecs::entity playerId);
#endif
