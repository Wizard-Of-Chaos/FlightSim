#include "GuiLoadoutMenu.h"
#include "GuiController.h"
#include "GameStateController.h"
#include <iostream>

ButtonPair GuiLoadoutMenu::createButtonPair(u32 num, position2di pos)
{
	ButtonPair pair;
	auto newPos = pos;
	pair.left = guienv->addButton(rect<s32>(pos, buttonSize), root, -1, L"<");
	newPos.X = pos.X + buf + buttonHoriz;

	pair.name = guienv->addStaticText(L"", rect<s32>(newPos, textSize), false, true, root, -1);
	newPos.X = pos.X + buf * 2 + buttonHoriz + textHoriz;

	pair.right = guienv->addButton(rect<s32>(newPos, buttonSize), root, -1, L">");

	if (num != -1) {
		pair.left->setName(std::to_string(num).c_str());
		pair.name->setName(std::to_string(num).c_str());
		pair.right->setName(std::to_string(num).c_str());
	}
	setMetalButton(pair.left);
	setMetalButton(pair.right);
	setUIText(pair.name);

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

	shipDescription = guienv->addStaticText(L"", rect<s32>(position2di(buf, baseSize.Height / 2 + buf + buttonVert), dimension2du(textHoriz + buttonHoriz * 2, textVert * 3)), false, true, root, -1);
	wepDescription = guienv->addStaticText(L"", rect<s32>(position2di(buf, buf * 2 + textVert), dimension2du(buttonHoriz * 2 + textHoriz + buf * 2, baseSize.Height / 3 - buf)), false, true, root, -1);
	returnToMain = guienv->addButton(rect<s32>(position2di(buf, buf), textSize), root, -1, L"Return", L"Stop editing loadout.");

	for (u32 i = 0; i < MAX_HARDPOINTS; ++i) {
		u32 yval = (buf + buf * i) + (buttonVert * i);
		u32 left = 3 + (3 * i) - 2;
		u32 center = 3 + (3 * i) - 3;
		u32 right = 3 + (3 * i) - 1;
		weaponButtons[i] = createButtonPair(i, position2di(baseSize.Width / 2, yval));
		guiController->setCallback(weaponButtons[i].left, std::bind(&GuiLoadoutMenu::onWeaponChangeLeft, this, std::placeholders::_1));
		guiController->setCallback(weaponButtons[i].right, std::bind(&GuiLoadoutMenu::onWeaponChangeRight, this, std::placeholders::_1));
		guiController->setCallback(weaponButtons[i].name, std::bind(&GuiLoadoutMenu::onWeaponHover, this, std::placeholders::_1));
	}

	shipButtons = createButtonPair(-1, position2di(buf, baseSize.Height / 2));

	guiController->setCallback(shipButtons.left, std::bind(&GuiLoadoutMenu::onShipChangeLeft, this, std::placeholders::_1));
	guiController->setCallback(shipButtons.right, std::bind(&GuiLoadoutMenu::onShipChangeRight, this, std::placeholders::_1));

	physWeaponButtons = createButtonPair(MAX_HARDPOINTS+1, position2di(baseSize.Width / 2, (buf + buf * 7) + (buttonVert * 7)));
	physWeaponButtons.name->setText(wstr(stateController->physWeaponData[stateController->playerPhysWeapon]->name).c_str());

	guiController->setCallback(physWeaponButtons.left, std::bind(&GuiLoadoutMenu::onWeaponChangeLeft, this, std::placeholders::_1));
	guiController->setCallback(physWeaponButtons.right, std::bind(&GuiLoadoutMenu::onWeaponChangeRight, this, std::placeholders::_1));
	guiController->setCallback(physWeaponButtons.name, std::bind(&GuiLoadoutMenu::onWeaponHover, this, std::placeholders::_1));

	setUIText(shipDescription);
	setUIText(wepDescription);
	setMetalButton(returnToMain);

	guiController->setCallback(returnToMain, std::bind(&GuiLoadoutMenu::onReturn, this, std::placeholders::_1));
	hide();
}

void GuiLoadoutMenu::show()
{
	root->setRelativePosition(rect<s32>(position2di(0, 0), driver->getScreenSize()));
	root->setVisible(true);
	for (u32 i = 0; i < MAX_HARDPOINTS; ++i) {
		weaponButtons[i].name->setText(L"");
	}

	if (stateController->inCampaign) {
		Campaign& camp = stateController->campaign;
		ShipInstance& ship = *camp.player->assignedShip;

		shipButtons.name->setText(wstr(stateController->shipData[ship.ship.shipDataId]->name).c_str());
		shipDescription->setText(wstr(stateController->shipData[ship.ship.shipDataId]->description).c_str());
		physWeaponButtons.name->setText(wstr(stateController->physWeaponData[ship.physWep.wepDataId]->name).c_str());
		for (u32 i = 0; i < ship.ship.hardpointCount; ++i) {
			weaponButtons[i].name->setText(wstr(stateController->weaponData[ship.weps[i].wepDataId]->name).c_str());
		}
	} else {
		shipButtons.name->setText(wstr(stateController->shipData[stateController->playerShip]->name).c_str());
		shipDescription->setText(wstr(stateController->shipData[stateController->playerShip]->description).c_str());
		physWeaponButtons.name->setText(wstr(stateController->physWeaponData[stateController->playerPhysWeapon]->name).c_str());
		for (u32 i = 0; i < stateController->shipData[stateController->playerShip]->shipComponent.hardpointCount; ++i) {
			weaponButtons[i].name->setText(wstr(stateController->weaponData[stateController->playerWeapons[i]]->name).c_str());
		}
	}
}

void GuiLoadoutMenu::setShipNameAndDesc(u32 shipId)
{
	stateController->playerShip = shipId;
	std::string name = stateController->shipData[shipId]->name;
	std::string desc = stateController->shipData[shipId]->description;

	shipButtons.name->setText(std::wstring(name.begin(), name.end()).c_str());
	shipDescription->setText(std::wstring(desc.begin(), desc.end()).c_str());

	for (u32 i = 0; i < stateController->shipData[shipId]->shipComponent.hardpointCount; ++i) {
		stateController->playerWeapons[i] = WEAPONID_NONE;
		ButtonPair p = weaponButtons[i];
		WeaponData* data = stateController->weaponData[0];
		weaponButtons[i].name->setText(std::wstring(data->name.begin(), data->name.end()).c_str());
	}
	for (u32 i = stateController->shipData[shipId]->shipComponent.hardpointCount; i < MAX_HARDPOINTS; ++i) {
		stateController->playerWeapons[i] = WEAPONID_INVALID;
		ButtonPair p = weaponButtons[i];
		weaponButtons[i].name->setText(L"");
	}
}

bool GuiLoadoutMenu::onShipChange(const SEvent& event, bool right)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	u32 shipId = stateController->playerShip;
	if (right) ++shipId;
	else {
		if (shipId == 0) shipId = stateController->shipData.size() - 1;
		else --shipId;
	}
	if (shipId == stateController->shipData.size()) shipId = 0;
	setShipNameAndDesc(shipId);
	return false;
}
bool GuiLoadoutMenu::onShipChangeLeft(const SEvent& event)
{
	return onShipChange(event, false);
}
bool GuiLoadoutMenu::onShipChangeRight(const SEvent& event)
{
	return onShipChange(event, true);
}

bool GuiLoadoutMenu::onWeaponChange(const SEvent& event, bool right)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	u32 num = std::stoi(event.GUIEvent.Caller->getName());

	if (num >= stateController->shipData[stateController->playerShip]->shipComponent.hardpointCount && num != MAX_HARDPOINTS + 1) return true;
	u32 wepId = stateController->playerPhysWeapon;
	if (num != MAX_HARDPOINTS + 1) {
		wepId = stateController->playerWeapons[num];
	}
	if (right) {
		++wepId;
		if (num == MAX_HARDPOINTS + 1 && wepId == stateController->physWeaponData.size()) wepId = 0;
		else if (num != MAX_HARDPOINTS + 1 && wepId == stateController->weaponData.size()) wepId = 0;
	}
	else {
		if (num == MAX_HARDPOINTS + 1 && wepId == 0) wepId = stateController->physWeaponData.size() - 1;
		else if (wepId == 0) wepId = stateController->weaponData.size() - 1;
		else --wepId;
	}
	WeaponData* data = stateController->weaponData[0];
	if (num == MAX_HARDPOINTS + 1) {
		stateController->playerPhysWeapon = wepId;
		data = stateController->physWeaponData[wepId];
		physWeaponButtons.name->setText(wstr(data->name).c_str());
	}
	else {
		stateController->playerWeapons[num] = wepId;
		data = stateController->weaponData[wepId];
		weaponButtons[num].name->setText(wstr(data->name).c_str());
	}

	return false;
}

bool GuiLoadoutMenu::onWeaponChangeLeft(const SEvent& event)
{
	return onWeaponChange(event, false);

}
bool GuiLoadoutMenu::onWeaponChangeRight(const SEvent& event)
{
	return onWeaponChange(event, true);
}

bool GuiLoadoutMenu::onWeaponHover(const SEvent& event)
{
	u32 num = std::stoi(event.GUIEvent.Caller->getName());

	if (num >= stateController->shipData[stateController->playerShip]->shipComponent.hardpointCount && num != MAX_HARDPOINTS +1) return true;

	if (event.GUIEvent.EventType == EGET_ELEMENT_HOVERED) {
		WeaponData* data = stateController->weaponData[stateController->playerWeapons[num]];
		if (num == MAX_HARDPOINTS+1) {
			data = stateController->physWeaponData[stateController->playerPhysWeapon];
		}
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
	if (stateController->inCampaign) {
		guiController->setActiveDialog(GUI_CAMPAIGN_MENU);
	}
	else {
		guiController->setActiveDialog(GUI_MAIN_MENU);
	}
	return false;
}