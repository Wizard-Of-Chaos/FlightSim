#pragma once

#ifndef PLAYERCOMPONENT_H
#define PLAYERCOMPONENT_H

#include "BaseHeader.h"

#include <vector>
#include <unordered_map>
#include <tuple>

class HUDElement;
class HUDContact;

struct BulletRigidBodyComponent;
struct FactionComponent;
/*
* The player component stores things that are exclusive to the player and get adjusted by the player.
* It stores the camera scene node, which allows the player to actually view the scene, the target scene node,
* which is tied to the IrrlichtComponent of the player ship and updates to allow the camera to rotate,
* and it stores the HUD for the player's ship. 

  The HUD is represented in the hud folder, and includes things derived
* from the base HUD element class. The HUD vector stores all HUD elements, and each HUD element has its own
* update call. Thanks, OOP design!
*/
struct PlayerComponent
{
	ICameraSceneNode* camera;
	ISceneNode* target;
	//values for how much the camera swings around
	f32 slerpFactor = .009f;
	f32 velocityFactor = .01f;

	//hud
	IGUIElement* rootHUD;
	std::vector<HUDElement*> HUD;

	vector3df thrust;
	vector3df rotation;

	std::unordered_map<flecs::entity_t, HUDContact*> trackedContacts;

	void removeContact(HUDContact* contact);
	void removeHUDElement(HUDElement* element);
};

#endif