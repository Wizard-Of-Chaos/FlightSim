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

	returnToMain = env->addButton(rect<s32>(position2di(buf, buf), textSize), root, LOADOUTMENU_RETURN_TO_MAIN, L"Return to Main", L"Return to the main menu.");

	for (int i = 0; i < MAX_HARDPOINTS; ++i) {
		createButtonPair(i);
	}

	scaleAlign(shipL);
	scaleAlign(shipR);
	scaleAlign(ship);
	scaleAlign(shipDescription);

	scaleAlign(returnToMain);
	shipL->setScaleImage();
	shipR->setScaleImage();
	returnToMain->setScaleImage();

	ship->setOverrideColor(SColor(255, 200, 200, 200));
	ship->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
	shipDescription->setOverrideColor(SColor(255, 200, 200, 200));
	shipDescription->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
	hide();

	guiController->setCallback(returnToMain, std::bind(&GuiLoadoutMenu::onReturn, this, std::placeholders::_1));
}

bool GuiLoadoutMenu::onShipChangeLeft(const SEvent& event)
{
	return false;
}
bool GuiLoadoutMenu::onShipChangeRight(const SEvent& event)
{
	return false;
}
bool GuiLoadoutMenu::onWeaponChangeLeft(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	u32 num = std::stoi(event.GUIEvent.Caller->getName());
	return false;
}
bool GuiLoadoutMenu::onWeaponChangeRight(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	u32 num = std::stoi(event.GUIEvent.Caller->getName());
	return false;
}
bool GuiLoadoutMenu::onReturn(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;

	guiController->setActiveDialog(GUI_MAIN_MENU);
	return false;
}