#include "IrrlichtUtils.h"
#include <string>
#include <iostream>

vector3df getNodeForward(ISceneNode* node)
{
	return node->getAbsoluteTransformation().getRotationDegrees().rotationToDirection(vector3df(0, 0, 1));
}

vector3df getNodeBackward(ISceneNode* node)
{
	return -getNodeForward(node);
}

vector3df getNodeUp(ISceneNode* node)
{
	return node->getAbsoluteTransformation().getRotationDegrees().rotationToDirection(vector3df(0, 1, 0));
}

vector3df getNodeDown(ISceneNode* node)
{
	return -getNodeUp(node);
}

vector3df getNodeRight(ISceneNode* node)
{
	return node->getAbsoluteTransformation().getRotationDegrees().rotationToDirection(vector3df(1, 0, 0));
}

vector3df getNodeLeft(ISceneNode* node)
{
	return -getNodeRight(node);
}

EntityId strToId(std::string id)
{
	return std::stoull(id);
}
std::string idToStr(EntityId id)
{
	return std::to_string(id);
}

void scaleAlign(IGUIElement* elem)
{
	elem->setAlignment(EGUIA_SCALE, EGUIA_SCALE, EGUIA_SCALE, EGUIA_SCALE);
}

void setUIText(IGUIStaticText* text)
{
	scaleAlign(text);
	text->setOverrideColor(SColor(255, 200, 200, 200));
	text->setOverrideFont(guienv->getFont("fonts/KomikaDisplayKaps14px/KomikaDisplayKaps14px.xml"));
	text->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
}

void setButtonImg(IGUIButton* elem)
{
	scaleAlign(elem);
	elem->setImage(driver->getTexture("ui/button1.png"));
	elem->setPressedImage(driver->getTexture("ui/button2.png"));
	elem->setScaleImage(true);
	elem->setOverrideFont(guienv->getFont("fonts/KomikaDisplayKaps14px/KomikaDisplayKaps14px.xml"));
}

std::string fprecis(f32 num, s32 round)
{
	std::string ret = std::to_string(num);
	ret.resize(ret.size() - round);
	return ret;
}

std::wstring wstr(std::string& str)
{
	return std::wstring(str.begin(), str.end());
}

