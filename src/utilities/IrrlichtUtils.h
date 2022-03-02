#pragma once

#ifndef IRRLICHTUTILS_H
#define IRRLICHTUTILS_H
#include "BaseHeader.h"
#include "ECS.h"

/*
* Utility functions for use with Irrlicht nodes to determine direction.
* Also includes functions to yank out the EntityId associated with an Irrlicht node.
*/

//Normalized forward vector for a given node.
vector3df getNodeForward(ISceneNode* node);
//Normalized backward vector for a given node.
vector3df getNodeBackward(ISceneNode* node);
//Normalized left vector for a given node.
vector3df getNodeLeft(ISceneNode* node);
//Normalized right vector for a given node.
vector3df getNodeRight(ISceneNode* node);
//Normalized up vector for a given node.
vector3df getNodeUp(ISceneNode* node);
//Normalized down vector for a given node.
vector3df getNodeDown(ISceneNode* node);

//Transforms a string to an EntityId.
EntityId strToId(std::string id);
//Transforms an EntityId to a string.
std::string idToStr(EntityId id);
//Convenience function to call on a GUI element to force it to scale with its root node.
void scaleAlign(IGUIElement* elem);

//Convenience funtion to return a string that is a rounded float. Thank you, GUI work.
//Chops off the last so-many digits, specified by "round".
std::string fprecis(f32 num, s32 round);
#endif