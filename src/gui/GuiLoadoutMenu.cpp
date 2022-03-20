#include "GuiLoadoutMenu.h"
#include "GuiController.h"
#include "GameStateController.h"

void GuiLoadoutMenu::init()
{
	u32 verticalSlice = baseSize.Height / 10;
	u32 horizontalSlice = baseSize.Width / 5;
	dimension2du buttonSize(horizontalSlice, verticalSlice);
	GameStateController* stCtrl = guiController->stateController;
	u32 buf = 8;
	std::wstring ship(stCtrl->playerShip.begin(), stCtrl->playerShip.end());
	std::wstring desc(stCtrl->shipData[stCtrl->playerShip]->description.begin(), stCtrl->shipData[stCtrl->playerShip]->description.end());
	std::wstring weps[MAX_HARDPOINTS];
	for (int i = 0; i < stCtrl->shipData[stCtrl->playerShip]->shipComponent.hardpointCount; ++i) {
		weps[i] = std::wstring(stCtrl->playerWeapons[i].begin(), stCtrl->playerWeapons[i].end());
	}
	shipSelect = guiController->guienv->addButton(rect<s32>(position2di(buf, buf), buttonSize), 
		root, LOADOUTMENU_SHIP_SELECT, ship.c_str(), L"Your current ship.");
	shipDescription = guiController->guienv->addStaticText(desc.c_str(),
		rect<s32>(position2di(buf*2 + horizontalSlice, buf), dimension2du(baseSize.Width - (buf*2 + horizontalSlice), verticalSlice)),
		false, true, root);

	weapon0 = guiController->guienv->addButton(rect<s32>(position2di(buf, buf*2 + verticalSlice), buttonSize),
		root, LOADOUTMENU_WEAPON_0, weps[0].c_str(), L"Weapon for slot 0.");
	weapon1 = guiController->guienv->addButton(rect<s32>(position2di(buf*2 + horizontalSlice, buf*2 + verticalSlice), buttonSize),
		root, LOADOUTMENU_WEAPON_1, weps[1].c_str(), L"Weapon for slot 1.");
	weapon2 = guiController->guienv->addButton(rect<s32>(position2di(buf*3  + horizontalSlice*2, buf*2 + verticalSlice), buttonSize),
		root, LOADOUTMENU_WEAPON_2, weps[2].c_str(), L"Weapon for slot 2.");
	weapon3 = guiController->guienv->addButton(rect<s32>(position2di(buf * 4 + horizontalSlice*3, buf * 2 + verticalSlice), buttonSize),
		root, LOADOUTMENU_WEAPON_3, weps[3].c_str(), L"Weapon for slot 3.");
	weapon4 = guiController->guienv->addButton(rect<s32>(position2di(buf, buf*3 + verticalSlice*2), buttonSize),
		root, LOADOUTMENU_WEAPON_4, weps[4].c_str(), L"Weapon for slot 4.");
	weapon5 = guiController->guienv->addButton(rect<s32>(position2di(buf * 2 + horizontalSlice, buf * 3 + verticalSlice * 2), buttonSize),
		root, LOADOUTMENU_WEAPON_5, weps[5].c_str(), L"Weapon for slot 5.");
	weapon6 = guiController->guienv->addButton(rect<s32>(position2di(buf * 3 + horizontalSlice*2, buf * 3 + verticalSlice * 2), buttonSize),
		root, LOADOUTMENU_WEAPON_6, weps[6].c_str(), L"Weapon for slot 6.");
	weapon7 = guiController->guienv->addButton(rect<s32>(position2di(buf * 4 + horizontalSlice*3, buf * 3 + verticalSlice * 2), buttonSize),
		root, LOADOUTMENU_WEAPON_7, weps[7].c_str(), L"Weapon for slot 7.");

	returnToMain = guiController->guienv->addButton(rect<s32>(position2di(buf, buf * 6 + verticalSlice * 3), buttonSize),
		root, LOADOUTMENU_RETURN_TO_MAIN, L"Return To Main", L"Return to the main menu.");

	scaleAlign(shipSelect);
	scaleAlign(returnToMain);
	scaleAlign(shipDescription);
	scaleAlign(weapon0);
	scaleAlign(weapon1);
	scaleAlign(weapon2);
	scaleAlign(weapon3);
	scaleAlign(weapon4);
	scaleAlign(weapon5);
	scaleAlign(weapon6);
	scaleAlign(weapon7);

	shipDescription->setOverrideColor(SColor(255, 200, 200, 200));

	shipSelect->setScaleImage();
	returnToMain->setScaleImage();
	weapon0->setScaleImage();
	weapon1->setScaleImage();
	weapon2->setScaleImage();
	weapon3->setScaleImage();
	weapon4->setScaleImage();
	weapon5->setScaleImage();
	weapon6->setScaleImage();
	weapon7->setScaleImage();

	//set callbacks here
	guiController->setCallback(returnToMain, std::bind(&GuiLoadoutMenu::onReturn, this, std::placeholders::_1));
	guiController->setCallback(shipSelect, std::bind(&GuiLoadoutMenu::onShipSelect, this, std::placeholders::_1));
	guiController->setCallback(weapon0, std::bind(&GuiLoadoutMenu::onWeaponSelect, this, std::placeholders::_1));
	guiController->setCallback(weapon1, std::bind(&GuiLoadoutMenu::onWeaponSelect, this, std::placeholders::_1));
	guiController->setCallback(weapon2, std::bind(&GuiLoadoutMenu::onWeaponSelect, this, std::placeholders::_1));
	guiController->setCallback(weapon3, std::bind(&GuiLoadoutMenu::onWeaponSelect, this, std::placeholders::_1));
	guiController->setCallback(weapon4, std::bind(&GuiLoadoutMenu::onWeaponSelect, this, std::placeholders::_1));
	guiController->setCallback(weapon5, std::bind(&GuiLoadoutMenu::onWeaponSelect, this, std::placeholders::_1));
	guiController->setCallback(weapon6, std::bind(&GuiLoadoutMenu::onWeaponSelect, this, std::placeholders::_1));
	guiController->setCallback(weapon7, std::bind(&GuiLoadoutMenu::onWeaponSelect, this, std::placeholders::_1));

	hide();
}
bool GuiLoadoutMenu::onShipSelect(const SEvent& event)
{
	return false;
}
bool GuiLoadoutMenu::onWeaponSelect(const SEvent& event)
{
	return false;
}
bool GuiLoadoutMenu::onReturn(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;

	guiController->setActiveDialog(GUI_MAIN_MENU);
	return false;
}