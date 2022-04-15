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

ButtonPair GuiLoadoutMenu::createButtonPair(u32 num, position2di pos, u32 left, u32 right, u32 cent)
{
	IGUIEnvironment* env = guiController->guienv;

	ButtonPair pair;
	auto newPos = pos;
	pair.buttonL = env->addButton(rect<s32>(pos, buttonSize), root, left, L"<");
	newPos.X = pos.X + buf + buttonHoriz;

	pair.name = env->addStaticText(L"", rect<s32>(newPos, textSize), false, true, root, cent);
	newPos.X = pos.X + buf * 2 + buttonHoriz + textHoriz;

	pair.buttonR = env->addButton(rect<s32>(newPos, buttonSize), root, right, L">");

	if (num != -1) {
		pair.name->setText(weps[num].c_str());
		pair.buttonL->setName(std::to_string(num).c_str());
		pair.name->setName(std::to_string(num).c_str());
		pair.buttonR->setName(std::to_string(num).c_str());
	}
	scaleAlign(pair.buttonL);
	scaleAlign(pair.buttonR);
	scaleAlign(pair.name);
	pair.buttonL->setScaleImage();
	pair.buttonR->setScaleImage();
	pair.name->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
	pair.name->setOverrideColor(SColor(255, 200, 200, 200));

	return pair;
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
	shipDescription = env->addStaticText(desc.c_str(), rect<s32>(position2di(buf, baseSize.Height / 2 + buf + buttonVert), dimension2du(textHoriz + buttonHoriz*2, textVert*3)), false, true, root, LOADOUTMENU_SHIP_DESC);
	wepDescription = env->addStaticText(L"", rect<s32>(position2di(buf, buf * 2 + textVert), dimension2du(buttonHoriz * 2 + textHoriz + buf * 2, baseSize.Height / 3 - buf)), false, true, root, LOADOUTMENU_WEP_DESC);
	returnToMain = env->addButton(rect<s32>(position2di(buf, buf), textSize), root, LOADOUTMENU_RETURN_TO_MAIN, L"Return to Main", L"Return to the main menu.");

	for (u32 i = 0; i < MAX_HARDPOINTS; ++i) {
		u32 yval = (buf + buf * i) + (buttonVert * i);
		u32 left = 3 + (3 * i) - 2;
		u32 center = 3 + (3 * i) - 3;
		u32 right = 3 + (3 * i) - 1;
		weaponButtons[i] = createButtonPair(i, position2di(baseSize.Width / 2, yval), left, right, center);
		guiController->setCallback(weaponButtons[i].buttonL, std::bind(&GuiLoadoutMenu::onWeaponChangeLeft, this, std::placeholders::_1));
		guiController->setCallback(weaponButtons[i].buttonR, std::bind(&GuiLoadoutMenu::onWeaponChangeRight, this, std::placeholders::_1));
		guiController->setCallback(weaponButtons[i].name, std::bind(&GuiLoadoutMenu::onWeaponHover, this, std::placeholders::_1));
	}

	shipButtons = createButtonPair(-1, position2di(buf, baseSize.Height / 2), LOADOUTMENU_SHIP_SELECT_L, LOADOUTMENU_SHIP_SELECT_R, LOADOUTMENU_SHIP_SELECT);
	shipButtons.name->setText(name.c_str());
	guiController->setCallback(shipButtons.buttonL, std::bind(&GuiLoadoutMenu::onShipChangeLeft, this, std::placeholders::_1));
	guiController->setCallback(shipButtons.buttonR, std::bind(&GuiLoadoutMenu::onShipChangeRight, this, std::placeholders::_1));

	physWeaponButtons = createButtonPair(-1, position2di(baseSize.Width / 2, (buf + buf * 7) + (buttonVert * 7)), LOADOUTMENU_PHYSWEP_L, LOADOUTMENU_PHYSWEP_R, LOADOUTMENU_PHYSWEP);
	physWeaponButtons.name->setText(wstr(stCtrl->physWeaponData[stCtrl->playerPhysWeapon]->name).c_str());
	guiController->setCallback(physWeaponButtons.buttonL, std::bind(&GuiLoadoutMenu::onPhysWeaponChangeLeft, this, std::placeholders::_1));
	guiController->setCallback(physWeaponButtons.buttonR, std::bind(&GuiLoadoutMenu::onPhysWeaponChangeRight, this, std::placeholders::_1));
	guiController->setCallback(physWeaponButtons.name, std::bind(&GuiLoadoutMenu::onPhysWeaponHover, this, std::placeholders::_1));

	scaleAlign(shipDescription);
	scaleAlign(wepDescription);

	scaleAlign(returnToMain);
	returnToMain->setScaleImage();
	guiController->setCallback(returnToMain, std::bind(&GuiLoadoutMenu::onReturn, this, std::placeholders::_1));

	shipDescription->setOverrideColor(SColor(255, 200, 200, 200));
	shipDescription->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
	wepDescription->setOverrideColor(SColor(255, 200, 200, 200));
	wepDescription->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
	hide();
}

void GuiLoadoutMenu::setShipNameAndDesc(u32 shipId)
{
	GameStateController* stCtrl = guiController->stateController;
	stCtrl->playerShip = shipId;
	std::string name = stCtrl->shipData[shipId]->name;
	std::string desc = stCtrl->shipData[shipId]->description;
	shipButtons.name->setText(std::wstring(name.begin(), name.end()).c_str());
	shipDescription->setText(std::wstring(desc.begin(), desc.end()).c_str());

	for (u32 i = 0; i < stCtrl->shipData[shipId]->shipComponent.hardpointCount; ++i) {
		stCtrl->playerWeapons[i] = WEAPONID_NONE;
		ButtonPair p = weaponButtons[i];
		WeaponData* data = stCtrl->weaponData[0];
		weaponButtons[i].name->setText(std::wstring(data->name.begin(), data->name.end()).c_str());
	}
	for (u32 i = stCtrl->shipData[shipId]->shipComponent.hardpointCount; i < MAX_HARDPOINTS; ++i) {
		stCtrl->playerWeapons[i] = WEAPONID_INVALID;
		ButtonPair p = weaponButtons[i];
		weaponButtons[i].name->setText(L"");
	}
}

bool GuiLoadoutMenu::onShipChangeLeft(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	u32 shipId = guiController->stateController->playerShip;
	std::cout << shipId;
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
	if (shipId == guiController->stateController->shipData.size() - 1) {
		shipId = 0;
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
	weaponButtons[num].name->setText(std::wstring(data->name.begin(), data->name.end()).c_str());

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
	weaponButtons[num].name->setText(std::wstring(data->name.begin(), data->name.end()).c_str());

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

bool GuiLoadoutMenu::onPhysWeaponChangeLeft(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	GameStateController* stCtrl = guiController->stateController;
	u32 wepId = stCtrl->playerPhysWeapon;
	if (wepId == 0) {
		wepId = stCtrl->physWeaponData.size() - 1;
	}
	else {
		--wepId;
	}
	stCtrl->playerPhysWeapon = wepId;
	WeaponData* data = stCtrl->physWeaponData[wepId];
	physWeaponButtons.name->setText(std::wstring(data->name.begin(), data->name.end()).c_str());
	return true;
}
bool GuiLoadoutMenu::onPhysWeaponChangeRight(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	GameStateController* stCtrl = guiController->stateController;
	u32 wepId = stCtrl->playerPhysWeapon;
	if (wepId == stCtrl->physWeaponData.size() - 1) {
		wepId = 0;
	}
	else {
		++wepId;
	}
	stCtrl->playerPhysWeapon = wepId;
	WeaponData* data = stCtrl->physWeaponData[wepId];
	physWeaponButtons.name->setText(std::wstring(data->name.begin(), data->name.end()).c_str());
	return true;
	return true;
}
bool GuiLoadoutMenu::onPhysWeaponHover(const SEvent& event)
{
	GameStateController* stCtrl = guiController->stateController;
	if (event.GUIEvent.EventType == EGET_ELEMENT_HOVERED) {
		WeaponData* data = stCtrl->physWeaponData[stCtrl->playerPhysWeapon];
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