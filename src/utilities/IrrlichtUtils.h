#pragma once

#ifndef IRRLICHTUTILS_H
#define IRRLICHTUTILS_H
#include "BaseHeader.h"

/*
* Utility functions for use with Irrlicht nodes to determine direction.
* Also includes functions to yank out the EntityId associated with an Irrlicht node.
*/

//The different types of ID an irrlicht node might have. Used to determine whether or not a player can
//"select" the object (or whether or not an AI can avoid the object). Tack on more if you need them.
enum SELECTIONS
{
	ID_IsNotSelectable = 0,
	ID_IsSelectable = 1 << 0,
	ID_IsAvoidable = 1 << 1
};

enum TEXT_SIZE
{
	TXT_SMALL,
	TXT_MED,
	TXT_LARGE
};

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
flecs::entity strToId(std::string id);
//Transforms an EntityId to a string.
std::string idToStr(flecs::entity id);
//Convenience function to call on a GUI element to force it to scale with its root node.
void scaleAlign(IGUIElement* elem);
//Convenience function to call on a GUI image to force it to scale with its root node.
void scaleAlign(IGUIImage* img);
//Sets the text element to the current UI font and color.
void setUIText(IGUIStaticText* text);
//Sets the button to a metallic button.
void setMetalButton(IGUIButton* elem);
//Sets the button to a holo-button, without a background if bg is false and with a background otherwise.
void setHoloButton(IGUIButton* elem, bool bg = false);
//Sets the button to a hybrid button, effectively a holo-button with a background.
void setHybridButton(IGUIButton* elem, bool bg = false);
//Sets the button to the repair button icon.
void setRepairButton(IGUIButton* elem);
//Sets the button to the reload button icon.
void setReloadButton(IGUIButton* elem);
/*
* Smoothly moves the GUI element from one position to the next.
* curTime is assumed to be a timer stored somewhere by whatever is calling this function. 
*/
bool smoothGuiMove(IGUIElement* elem, f32 animTime, f32& curTime, position2di desiredPos, position2di startPos, f32 dt);

bool smoothFade(IGUIElement* elem, f32 animTime, f32& curTime, f32 desiredAlpha, f32 startAlpha, f32 dt);

//Convenience funtion to return a string that is a rounded float. Thank you, GUI work.
//Chops off the last so-many digits, specified by "round".
std::string fprecis(f32 num, s32 round);

//Converts a string to a std::wstring (irrlicht text only accepts wide characters).
std::wstring wstr(std::string& str);
#endif