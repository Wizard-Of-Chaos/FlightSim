#include "GuiCampaignLoadoutMenu.h"
#include "GameStateController.h"
#include "GameController.h"
#include "GuiController.h"

void GuiCampaignLoadoutMenu::init()
{
	dimension2du buttonSize(160, 40);
	dimension2du smallButtonSize(60, 40);
	back = guienv->addButton(rect<s32>(position2di(0, 0), buttonSize), root, -1, L"Back", L"Return to campaign menu.");
	wepMenu = guienv->addImage(rect<s32>(position2di(10, 120), dimension2du(237, 319)), root);
	physWepMenu = guienv->addImage(rect<s32>(position2di(713, 120), dimension2du(237, 319)), root);
	wepMenu->setImage(driver->getTexture("ui/freestandingmenu.png"));
	physWepMenu->setImage(driver->getTexture("ui/freestandingmenu.png"));
	scaleAlign(wepMenu);
	scaleAlign(physWepMenu);
	setHybridButton(back, true);

	position2di pos(280, 140);
	position2di newpos;
	s32 buf = 8;
	for (u32 i = 0; i < MAX_HARDPOINTS; ++i) {
		newpos = pos;
		newpos.Y += (buf * i) + (buttonSize.Height * i);
		hardpoints[i] = guienv->addButton(rect<s32>(newpos, buttonSize), root, -1, L"Hardpoint", L"Hardpoint attachment for a weapon.");
		setHybridButton(hardpoints[i]);
		hardpoints[i]->setID((s32)i);
	}
	position2di rpos(520, 140);
	physWepHardpoint = guienv->addButton(rect<s32>(rpos, buttonSize), root, -1, L"Phys Wep", L"Hardpoint for the physics weapon.");
	physWepHardpoint->setID(PHYS_HARDPOINT);
	setHybridButton(physWepHardpoint);
	pos.Y -= 60;
	rpos.Y -= 60;
	rpos.X += 100;
	shipSelector.left = guienv->addButton(rect<s32>(pos, smallButtonSize), root, -1, L"<");
	shipSelector.right = guienv->addButton(rect<s32>(rpos, smallButtonSize), root, -1, L">");
	pos.X += smallButtonSize.Width;
	shipSelector.name = guienv->addStaticText(L"Ship Instance", rect<s32>(pos, dimension2du(280, 40)), false, true, root);

	setUIText(shipSelector.name);
	setMetalButton(shipSelector.left);
	setMetalButton(shipSelector.right);

	newpos.Y += buttonSize.Height + buf;
	shipInfo = guienv->addStaticText(L"Ship Information", rect<s32>(newpos, dimension2du(400, 70)), true, true, root);
	setUIText(shipInfo);

	guiController->setCallback(back, std::bind(&GuiCampaignLoadoutMenu::onBack, this, std::placeholders::_1));
	guiController->setCallback(shipSelector.left, std::bind(&GuiCampaignLoadoutMenu::onShipChangeLeft, this, std::placeholders::_1));
	guiController->setCallback(shipSelector.right, std::bind(&GuiCampaignLoadoutMenu::onShipChangeRight, this, std::placeholders::_1));
	hide();
}

bool GuiCampaignLoadoutMenu::onBack(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	guiController->setActiveDialog(GUI_CAMPAIGN_MENU);
	return false;
}

void GuiCampaignLoadoutMenu::show()
{
	root->setRelativePosition(rect<s32>(position2di(0, 0), driver->getScreenSize()));
	root->setVisible(true);

	//initialize first loadout visible as player ship'
	shipSelector.curPos = -1;
	displayShip(stateController->campaign.playerShip);
}

void GuiCampaignLoadoutMenu::displayShip(ShipInstance& inst)
{
	u32 shipId = inst.ship.shipDataId;
	ShipData* data = stateController->shipData[shipId];
	std::string name = data->name;
	if (shipSelector.curPos == -1) {
		name += " - Player";
	}
	shipSelector.name->setText(wstr(name).c_str());

	for (u32 i = 0; i < inst.ship.hardpointCount; ++i) {
		std::string wep = stateController->weaponData[inst.weps[i].wepDataId]->name;
		hardpoints[i]->setText(wstr(wep).c_str());
		hardpoints[i]->setVisible(true);
	}
	for (u32 i = inst.ship.hardpointCount; i < MAX_HARDPOINTS; ++i) {
		hardpoints[i]->setVisible(false);
	}
	std::string phys = stateController->physWeaponData[inst.physWep.wepDataId]->name;
	physWepHardpoint->setText(wstr(phys).c_str());

	std::string desc = name + "\n" + data->description + "\n HP: " + fprecis(inst.hp.health, 5) + "/" + fprecis(inst.hp.maxHealth, 5);
	shipInfo->setText(wstr(desc).c_str());
}

bool GuiCampaignLoadoutMenu::onShipChange(const SEvent& event, bool right)
{
	Campaign& camp = stateController->campaign;
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED || camp.availableShips.size() == 0) return true;
	if (right) {
		++shipSelector.curPos;
		if (shipSelector.curPos == camp.availableShips.size()) shipSelector.curPos = -1;
	}
	else {
		--shipSelector.curPos;
		if (shipSelector.curPos < -1) shipSelector.curPos = camp.availableShips.size()-1;
	}

	if (shipSelector.curPos <= -1) {
		displayShip(camp.playerShip);
	}
	else {
		displayShip(camp.availableShips[shipSelector.curPos]);
	}
	return false;
}
bool GuiCampaignLoadoutMenu::onShipChangeLeft(const SEvent& event)
{
	return onShipChange(event, false);
}
bool GuiCampaignLoadoutMenu::onShipChangeRight(const SEvent& event)
{
	return onShipChange(event, true);
}
bool GuiCampaignLoadoutMenu::onHardpointSelect(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	return false;
}
bool GuiCampaignLoadoutMenu::onWepSelect(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	return false;
}