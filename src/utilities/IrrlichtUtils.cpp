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

flecs::entity strToId(std::string id)
{
	flecs::entity_t num = std::stoull(id);
	return flecs::entity(game_world->get_world(), num);
}
std::string idToStr(flecs::entity id)
{
	return std::to_string(id.id());
}

void scaleAlign(IGUIElement* elem)
{
	elem->setAlignment(EGUIA_SCALE, EGUIA_SCALE, EGUIA_SCALE, EGUIA_SCALE);
}

void scaleAlign(IGUIImage* img)
{
	img->setAlignment(EGUIA_SCALE, EGUIA_SCALE, EGUIA_SCALE, EGUIA_SCALE);
	img->setScaleImage(true);
}

void setUIText(IGUIStaticText* text)
{
	scaleAlign(text);
	text->setOverrideColor(SColor(255, 200, 200, 200));
	text->setOverrideFont(guienv->getFont("fonts/KomikaDisplayKaps14px/KomikaDisplayKaps14px.xml"));
	text->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
}

void setMetalButton(IGUIButton* elem)
{
	scaleAlign(elem);
	elem->setImage(driver->getTexture("ui/button1.png"));
	elem->setPressedImage(driver->getTexture("ui/button2.png"));
	elem->setScaleImage(true);
	elem->setOverrideFont(guienv->getFont("fonts/KomikaDisplayKaps22px/KomikaDisplayKaps22px.xml"));
}

void setHoloButton(IGUIButton* elem, bool bg)
{
	scaleAlign(elem);
	elem->setUseAlphaChannel(true);
	if (bg) elem->setImage(driver->getTexture("ui/holobuttonbg1.png"));
	else elem->setImage(driver->getTexture("ui/holobutton1.png"));

	elem->setPressedImage(driver->getTexture("ui/holobuttonbg2.png"));
	elem->setScaleImage(true);
	elem->setOverrideFont(guienv->getFont("fonts/KomikaDisplayKaps22px/KomikaDisplayKaps22px.xml"));
	elem->setDrawBorder(false);
}

void setHybridButton(IGUIButton* elem, bool bg)
{
	scaleAlign(elem);
	elem->setUseAlphaChannel(true);
	if (bg) elem->setImage(driver->getTexture("ui/hybridbuttonbg1.png"));
	else elem->setImage(driver->getTexture("ui/hybridbutton1.png"));

	elem->setPressedImage(driver->getTexture("ui/hybridbutton2.png"));
	elem->setScaleImage(true);
	elem->setOverrideFont(guienv->getFont("fonts/KomikaDisplayKaps22px/KomikaDisplayKaps22px.xml"));
	elem->setDrawBorder(false);
}
void setRepairButton(IGUIButton* elem)
{
	scaleAlign(elem);
	elem->setUseAlphaChannel(true);
	elem->setDrawBorder(false);
	elem->setScaleImage(true);

	elem->setImage(driver->getTexture("ui/repair1.png"));
	elem->setPressedImage(driver->getTexture("ui/repair2.png"));
}
void setReloadButton(IGUIButton* elem)
{
	scaleAlign(elem);
	elem->setUseAlphaChannel(true);
	elem->setDrawBorder(false);
	elem->setScaleImage(true);

	elem->setImage(driver->getTexture("ui/reload1.png"));
	elem->setPressedImage(driver->getTexture("ui/reload2.png"));
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

bool smoothGuiMove(IGUIElement* elem, f32 animTime, f32& curTime, position2di desiredPos, position2di startPos, f32 dt)
{
	curTime += dt;
	if (curTime > animTime) curTime = animTime;
	s32 xDiff = startPos.X - desiredPos.X;
	s32 yDiff = startPos.Y - desiredPos.Y;
	s32 xMove = (s32)((dt / animTime) * xDiff);
	s32 yMove = (s32)((dt / animTime) * yDiff);
	elem->move(vector2di(xMove, yMove));
	if (animTime == curTime) {
		curTime = 0;
		return false;
	}
	return true;
}

bool smoothFade(IGUIElement* elem, f32 animTime, f32& curTime, f32 desiredAlpha, f32 startAlpha, f32 dt)
{
	return true;
}

