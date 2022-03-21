#include "GuiLoadoutMenu.h"
#include "GuiController.h"
#include "GameStateController.h"
#include <iostream>

void GuiLoadoutMenu::loadoutToWString()
{
	GameStateController* stCtrl = guiController->stateController;
	ShipData* shipdata = stCtrl->shipData[stCtrl->playerShip];

	name = std::wstring(shipdata->name.begin(), shipdata->name.end());
	desc = std::wstring(shipdata->description.begin(), shipdata->description.end());
	for (int i = 0; i < shipdata->shipComponent.hardpointCount; ++i) {
		std::string wep = stCtrl->weaponData[stCtrl->playerWeapons[i]]->name;
		weps[i] = std::wstring(wep.begin(), wep.end());
	}
}

void GuiLoadoutMenu::createButtonPair(u32 num)
{
	u32 yval = buf + (buf * num) + (buttonVert * num);
	IGUIEnvironment* env = guiController->guienv;

	weaponbuttons[num].buttonL = env->addButton(rect<s32>(position2di(baseSize.Width / 2, yval), buttonSize), root, 3+(3*num)-2, L"<");
	weaponbuttons[num].buttonL->setName(std::to_string(num).c_str());
	weaponbuttons[num].name = env->addStaticText(weps[num].c_str(), rect<s32>(position2di(baseSize.Width / 2 + buf + buttonHoriz, yval), textSize), false, true, root, 3 + (3 * num) - 3);
	weaponbuttons[num].name->setName(std::to_string(num).c_str());
	weaponbuttons[num].buttonR = env->addButton(rect<s32>(position2di(baseSize.Width / 2 + buf * 2 + buttonHoriz + textHoriz, yval), buttonSize), root, 3 + (3 * num) - 1, L">");
	weaponbuttons[num].buttonR->setName(std::to_string(num).c_str());

	scaleAlign(weaponbuttons[num].buttonL);
	scaleAlign(weaponbuttons[num].buttonR);
	scaleAlign(weaponbuttons[num].name);
	weaponbuttons[num].buttonL->setScaleImage();
	weaponbuttons[num].buttonR->setScaleImage();
	weaponbuttons[num].name->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
	weaponbuttons[num].name->setOverrideColor(SColor(255, 200, 200, 200));
	
	guiController->setCallback(weaponbuttons[num].buttonL, std::bind(&GuiLoadoutMenu::onWeaponChangeLeft, this, std::placeholders::_1));
	guiController->setCallback(weaponbuttons[num].buttonR, std::bind(&GuiLoadoutMenu::onWeaponChangeRight, this, std::placeholders::_1));
	guiController->setCallback(weaponbuttons[num].name, std::bind(&GuiLoadoutMenu::onWeaponHover, this, std::placeholders::_1));
}
void GuiLoadoutMenu::init()
{
	buttonVert = baseSize.Height / 10;
	buttonHoriz = baseSize.Width / 15;
	textVert = buttonVert;
	textHoriz = baseSize.Width/4;
	buttonSize = dimension2du(buttonHoriz, buttonVert);
	textSize = dimension2du(textHoriz, textVert);
	buf = 8;

	GameStateController* stCtrl = guiController->stateController;

	IGUIEnvironment* env = guiController->guienv;
	loadoutToWString();
	shipL = env->addButton(rect<s32>(position2di(buf, baseSize.Height / 2), buttonSize), root, LOADOUTMENU_SHIP_SELECT_L, L"<");
	shipR = env->addButton(rect<s32>(position2di(buf * 3 + textHoriz + buttonHoriz, baseSize.Height / 2), buttonSize), root, LOADOUTMENU_SHIP_SELECT_R, L">");
	ship = env->addStaticText(name.c_str(), rect<s32>(position2di(buf * 2 + buttonHoriz, baseSize.Height / 2), textSize), false, true, root, LOADOUTMENU_SHIP_SELECT);
	shipDescription = env->addStaticText(desc.c_str(), rect<s32>(position2di(buf, baseSize.Height / 2 + buf + buttonVert), dimension2du(textHoriz + buttonHoriz*2, textVert*3)), false, true, root, LOADOUTMENU_SHIP_DESC);
	wepDescription = env->addStaticText(L"", rect<s32>(position2di(buf, buf * 2 + textVert), dimension2du(buttonHoriz * 2 + textHoriz + buf * 2, baseSize.Height / 2 - buf)), false, true, root, LOADOUTMENU_WEP_DESC);
	returnToMain = env->addButton(rect<s32>(position2di(buf, buf), textSize), root, LOADOUTMENU_RETURN_TO_MAIN, L"Return to Main", L"Return to the main menu.");

	for (int i = 0; i < MAX_HARDPOINTS; ++i) {
		createButtonPair(i);
	}

	scaleAlign(shipL);
	scaleAlign(shipR);
	scaleAlign(ship);
	scaleAlign(shipDescription);
	scaleAlign(wepDescription);

	scaleAlign(returnToMain);
	shipL->setScaleImage();
	shipR->setScaleImage();
	returnToMain->setScaleImage();

	ship->setOverrideColor(SColor(255, 200, 200, 200));
	ship->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
	shipDescription->setOverrideColor(SColor(255, 200, 200, 200));
	shipDescription->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
	wepDescription->setOverrideColor(SColor(255, 200, 200, 200));
	wepDescription->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);

	guiController->setCallback(returnToMain, std::bind(&GuiLoadoutMenu::onReturn, this, std::placeholders::_1));
	guiController->setCallback(shipL, std::bind(&GuiLoadoutMenu::onShipChangeLeft, this, std::placeholders::_1));
	guiController->setCallback(shipR, std::bind(&GuiLoadoutMenu::onShipChangeRight, this, std::placeholders::_1));
	hide();
}

void GuiLoadoutMenu::setShipNameAndDesc(u32 shipId)
{
	GameStateController* stCtrl = guiController->stateController;
	stCtrl->playerShip = shipId;
	std::string name = stCtrl->shipData[shipId]->name;
	std::string desc = stCtrl->shipData[shipId]->description;
	ship->setText(std::wstring(name.begin(), name.end()).c_str());
	shipDescription->setText(std::wstring(desc.begin(), desc.end()).c_str());
}

bool GuiLoadoutMenu::onShipChangeLeft(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;

	u32 shipId = guiController->stateController->playerShip;
	if (shipId == 0) {
		shipId = guiController->stateController->shipData.size() - 1;
	}
	else {
		--shipId;
	}
	setShipNameAndDesc(shipId);
	return false;
}
bool GuiLoadoutMenu::onShipChangeRight(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	GameStateController* stCtrl = guiController->stateController;
	u32 shipId = guiController->stateController->playerShip;
	if (shipId == 0) {
		shipId = guiController->stateController->shipData.size() - 1;
	}
	else {
		++shipId;
	}
	setShipNameAndDesc(shipId);

	return false;
}
bool GuiLoadoutMenu::onWeaponChangeLeft(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	u32 num = std::stoi(event.GUIEvent.Caller->getName());
	GameStateController* stCtrl = guiController->stateController;
	if (num >= stCtrl->shipData[stCtrl->playerShip]->shipComponent.hardpointCount) return true;

	u32 wepId = stCtrl->playerWeapons[num];
	if (wepId == 0) {
		wepId = stCtrl->weaponData.size()-1;
	} else {
		--wepId;
	}
	stCtrl->playerWeapons[num] = wepId;
	WeaponData* data = stCtrl->weaponData[wepId];
	weaponbuttons[num].name->setText(std::wstring(data->name.begin(), data->name.end()).c_str());

	return false;
}
bool GuiLoadoutMenu::onWeaponChangeRight(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	u32 num = std::stoi(event.GUIEvent.Caller->getName());
	GameStateController* stCtrl = guiController->stateController;
	if (num >= stCtrl->shipData[stCtrl->playerShip]->shipComponent.hardpointCount) return true;

	u32 wepId = stCtrl->playerWeapons[num];
	if (wepId == stCtrl->weaponData.size() - 1) {
		wepId = 0;
	}
	else {
		++wepId;
	}
	stCtrl->playerWeapons[num] = wepId;
	WeaponData* data = stCtrl->weaponData[wepId];
	weaponbuttons[num].name->setText(std::wstring(data->name.begin(), data->name.end()).c_str());

	return false;
}

bool GuiLoadoutMenu::onWeaponHover(const SEvent& event)
{
	GameStateController* stCtrl = guiController->stateController;
	u32 num = std::stoi(event.GUIEvent.Caller->getName());
	if (num >= stCtrl->shipData[stCtrl->playerShip]->shipComponent.hardpointCount) return true;

	if (event.GUIEvent.EventType == EGET_ELEMENT_HOVERED) {
		WeaponData* data = stCtrl->weaponData[stCtrl->playerWeapons[num]];
		wepDescription->setText(std::wstring(data->description.begin(), data->description.end()).c_str());
		return false;
	}
	else if (event.GUIEvent.EventType == EGET_ELEMENT_LEFT) {
		wepDescription->setText(L"");
		return false;
	}
	return true;
}

bool GuiLoadoutMenu::onReturn(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;

	guiController->setActiveDialog(GUI_MAIN_MENU);
	return false;
}