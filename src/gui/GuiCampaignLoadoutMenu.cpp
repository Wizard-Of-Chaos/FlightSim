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
		guiController->setCallback(hardpoints[i], std::bind(&GuiCampaignLoadoutMenu::onHardpointSelect, this, std::placeholders::_1));
		newpos.X += buttonSize.Width;
		newpos.Y += 10;
		reloadHardpoints[i] = guienv->addButton(rect<s32>(newpos, dimension2du(20, 20)), root, (s32)i, L"", L"Reload this weapon.");
		setReloadButton(reloadHardpoints[i]);
		guiController->setCallback(reloadHardpoints[i], std::bind(&GuiCampaignLoadoutMenu::onReload, this, std::placeholders::_1));
		newpos.X -= buttonSize.Width;
		newpos.Y -= 10;
	}
	position2di rpos(520, 140);
	physWepHardpoint = guienv->addButton(rect<s32>(rpos, buttonSize), root, -1, L"Phys Wep", L"Hardpoint for the physics weapon.");
	physWepHardpoint->setID(PHYS_HARDPOINT);
	setHybridButton(physWepHardpoint);
	guiController->setCallback(physWepHardpoint, std::bind(&GuiCampaignLoadoutMenu::onHardpointSelect, this, std::placeholders::_1));
	pos.Y -= 60;
	rpos.Y -= 60;
	rpos.X += 100;
	shipSelector.left = guienv->addButton(rect<s32>(pos, smallButtonSize), root, -1, L"<");
	shipSelector.right = guienv->addButton(rect<s32>(rpos, smallButtonSize), root, -1, L">");
	rpos.Y += 10;
	rpos.X += smallButtonSize.Width;
	repair = guienv->addButton(rect<s32>(rpos, dimension2du(20, 20)), root, -1, L"", L"Repair this ship.");
	setRepairButton(repair);
	guiController->setCallback(repair, std::bind(&GuiCampaignLoadoutMenu::onRepair, this, std::placeholders::_1));

	pos.X += smallButtonSize.Width;
	shipSelector.name = guienv->addStaticText(L"Ship Instance", rect<s32>(pos, dimension2du(280, 40)), false, true, root);

	setUIText(shipSelector.name);
	setMetalButton(shipSelector.left);
	setMetalButton(shipSelector.right);

	newpos.Y += buttonSize.Height + buf;
	pos.Y = 0;
	pos.X -= smallButtonSize.Width;
	shipInfo = guienv->addStaticText(L"Ship Information", rect<s32>(pos, dimension2du(400, 70)), true, true, root);
	wepDesc = guienv->addStaticText(L"", rect<s32>(newpos, dimension2du(400, 70)), false, true, root);
	setUIText(wepDesc);
	setUIText(shipInfo);
	pos.X -= 100;
	pos.Y += 40;
	shipSelector.useShip = guienv->addButton(rect<s32>(pos, dimension2du(100, 25)), root, -1, L"Use this ship", L"Use this ship as your own.");
	setMetalButton(shipSelector.useShip);
	guiController->setCallback(shipSelector.useShip, std::bind(&GuiCampaignLoadoutMenu::onUseShip, this, std::placeholders::_1));

	wepMenuBg = guienv->addStaticText(L"", rect<s32>(position2di(20, 20), dimension2du(187, 267)), false, true, wepMenu);
	setUIText(wepMenuBg); //not really necessary; the bg just provides something to tape wep loadout stuff into
	physWepMenuBg = guienv->addStaticText(L"", rect<s32>(position2di(20, 20), dimension2du(187, 267)), false, true, physWepMenu);
	setUIText(physWepMenuBg);

	carrierInfoPanel = guienv->addImage(rect<s32>(position2di(758, 0), dimension2du(202, 88)), root);
	scaleAlign(carrierInfoPanel);
	carrierInfoPanel->setImage(driver->getTexture("ui/infopanel.png"));
	carrierInfo = guienv->addStaticText(L"", rect<s32>(position2di(35, 0), dimension2du(165, 65)), false, true, carrierInfoPanel);
	setUIText(carrierInfo);

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
	shipSelector.curPos = 0;
	displayShip(stateController->campaign.ships[0]);
	displayCarrierInfo();
}

void GuiCampaignLoadoutMenu::displayShip(ShipInstance* inst)
{
	u32 shipId = inst->ship.shipDataId;
	ShipData* data = stateController->shipData[shipId];
	std::string name = data->name;
	if (inst == stateController->campaign.player->assignedShip) {
		shipSelector.useShip->setVisible(false);
		name += " - Player";
	}
	else {
		shipSelector.useShip->setVisible(true);
	}
	shipSelector.name->setText(wstr(name).c_str());

	for (u32 i = 0; i < inst->ship.hardpointCount; ++i) {
		WeaponInfoComponent wep = stateController->weaponData[inst->weps[i].wepDataId]->weaponComponent;
		std::string name = stateController->weaponData[inst->weps[i].wepDataId]->name;
		hardpoints[i]->setText(wstr(name).c_str());
		hardpoints[i]->setVisible(true);
		if (wep.usesAmmunition) {
			reloadHardpoints[i]->setVisible(true);
		}
		else {
			reloadHardpoints[i]->setVisible(false);
		}

	}
	for (u32 i = inst->ship.hardpointCount; i < MAX_HARDPOINTS; ++i) {
		hardpoints[i]->setVisible(false);
		reloadHardpoints[i]->setVisible(false);
	}
	std::string phys = stateController->physWeaponData[inst->physWep.wepDataId]->name;
	physWepHardpoint->setText(wstr(phys).c_str());

	std::string desc = name + "\n" + data->description + "\n HP: " + fprecis(inst->hp.health, 5) + "/" + fprecis(inst->hp.maxHealth, 5);
	shipInfo->setText(wstr(desc).c_str());
}
void GuiCampaignLoadoutMenu::displayCarrierInfo()
{
	Campaign& camp = stateController->campaign;
	std::string txt = "Carrier Info \n Total repair capacity: " + fprecis(camp.totalRepairCapacity, 5) + "\n Total ammo available: " + std::to_string(camp.totalAmmunition);
	carrierInfo->setText(wstr(txt).c_str());
}

bool GuiCampaignLoadoutMenu::onShipChange(const SEvent& event, bool right)
{
	Campaign& camp = stateController->campaign;
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED || camp.ships.size() == 0) return true;
	if (right) {
		++shipSelector.curPos;
		if (shipSelector.curPos == camp.ships.size()) shipSelector.curPos = 0;
	}
	else {
		--shipSelector.curPos;
		if (shipSelector.curPos < 0) shipSelector.curPos = camp.ships.size()-1;
	}

	displayShip(getCurShip());
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

WepSelect GuiCampaignLoadoutMenu::buildWepSelect(WeaponInfoComponent& wep, position2di pos, bool phys)
{
	f32 screenRatioHoriz = 187.f / 960.f;
	f32 screenRatioVert = 40.f / 540.f;
	s32 width = (s32)(screenRatioHoriz * root->getAbsolutePosition().getWidth());
	s32 height = (s32)(screenRatioVert * root->getAbsolutePosition().getHeight());

	rect<s32> bgRect(pos, dimension2du(width, height));
	WepSelect sel;
	WeaponData* data = stateController->weaponData[wep.wepDataId];
	position2di npos(0, 0);
	position2di bpos(bgRect.getWidth() / 2, 0);
	IGUIStaticText* bg = wepMenuBg;

	if (phys) {
		bg = physWepMenuBg;
		npos = bpos;
		bpos = position2di(0, 0);
		data = stateController->physWeaponData[wep.wepDataId];
	}
	sel.bg = guienv->addStaticText(L"", bgRect, false, true, bg);
	std::string txt = data->name;
	if (data->weaponComponent.usesAmmunition) {
		txt += "\n Ammo: " + std::to_string(wep.ammunition + wep.maxClip) + "/" + std::to_string(data->weaponComponent.maxAmmunition + data->weaponComponent.maxClip);
	}
	sel.name = guienv->addStaticText(wstr(txt).c_str(), rect<s32>(npos, dimension2du(bgRect.getWidth() / 2, bgRect.getHeight())), false, true, sel.bg);
	sel.select = guienv->addButton(rect<s32>(bpos, dimension2du(bgRect.getWidth() / 2, bgRect.getHeight())), sel.bg, -1, L"Select", L"Select this weapon.");
	setUIText(sel.bg);
	setUIText(sel.name);
	setHoloButton(sel.select);

	return sel;
}

ShipInstance* GuiCampaignLoadoutMenu::getCurShip()
{
	return stateController->campaign.ships[shipSelector.curPos];
}

void GuiCampaignLoadoutMenu::displayWeaponList()
{
	rect<s32> screen = root->getAbsolutePosition();
	f32 screenRatioVert = 40 / 540.f;
	for (u32 i = 0; i < stateController->campaign.availableWeapons.size(); ++i) {
		WeaponInfoComponent cmp = stateController->campaign.availableWeapons[i];
		WepSelect selector = buildWepSelect(cmp, position2di(0, i * screenRatioVert * screen.getHeight()), false);
		selector.select->setID((s32)i);
		guiController->setCallback(selector.select, std::bind(&GuiCampaignLoadoutMenu::onWepSelect, this, std::placeholders::_1));
		weaponList.push_back(selector);
	}
}

void GuiCampaignLoadoutMenu::clearWeaponList()
{
	for (WepSelect sel : physWeaponList) {
		sel.name->remove();
		guiController->removeCallback(sel.select);
		sel.select->remove();
		sel.bg->remove();
	}
	physWeaponList.clear();
	for (WepSelect sel : weaponList) {
		sel.name->remove();
		guiController->removeCallback(sel.select);
		sel.select->remove();
		sel.bg->remove();
	}
	weaponList.clear();
}

void GuiCampaignLoadoutMenu::displayPhysWeaponList()
{
	rect<s32> screen = root->getAbsolutePosition();
	f32 screenRatioVert = 40 / 540.f;
	for (u32 i = 0; i < stateController->campaign.availablePhysWeapons.size(); ++i) {
		WeaponInfoComponent cmp = stateController->campaign.availablePhysWeapons[i];
		WepSelect selector = buildWepSelect(cmp, position2di(0, i * screenRatioVert * screen.getHeight()), true);
		selector.select->setID((s32)i);
		guiController->setCallback(selector.select, std::bind(&GuiCampaignLoadoutMenu::onPhysWepSelect, this, std::placeholders::_1));
		weaponList.push_back(selector);
	}
}

bool GuiCampaignLoadoutMenu::wepHover(const SEvent& event)
{
	u32 hardpoint = (u32)event.GUIEvent.Caller->getID();
	WeaponInfoComponent wep;
	if (event.GUIEvent.EventType == EGET_ELEMENT_HOVERED) {
		ShipInstance* inst = getCurShip();
		wep = inst->weps[hardpoint];
		if (event.GUIEvent.Caller->getID() == PHYS_HARDPOINT) {
			wep = inst->physWep;
		}
		u32 wepId = wep.wepDataId;
		WeaponData* data = stateController->weaponData[wepId];
		if (event.GUIEvent.Caller->getID() == PHYS_HARDPOINT) {
			data = stateController->physWeaponData[wepId];
		}
		std::string txt = data->name + "\n" + data->description;
		if (data->weaponComponent.usesAmmunition) {
			txt += "\n Ammo: " + std::to_string(wep.ammunition + wep.maxClip) + "/" + std::to_string(wep.maxAmmunition + wep.maxClip);
		}
		wepDesc->setText(wstr(txt).c_str());
		wepDesc->setVisible(true);
	}
	else if (event.GUIEvent.EventType == EGET_ELEMENT_LEFT) {
		wepDesc->setText(L"");
		wepDesc->setVisible(false);
	}
	return false;
}

bool GuiCampaignLoadoutMenu::onHardpointSelect(const SEvent& event)
{
	if (event.GUIEvent.EventType == EGET_ELEMENT_HOVERED || event.GUIEvent.EventType == EGET_ELEMENT_LEFT) return wepHover(event);

	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;

	IGUIButton* button = (IGUIButton*)event.GUIEvent.Caller;
	if (button->isPressed()) {
		button->setPressed(false);
		clearWeaponList();
		return false;
	}
	clearWeaponList();
	if (button->getID() == PHYS_HARDPOINT) {
		displayPhysWeaponList();
	} else {
		displayWeaponList();
	}
	button->setPressed(true);
	currentHardpoint = (u32)(button->getID());
	return false;
}

bool GuiCampaignLoadoutMenu::wepSelect(const SEvent& event, ShipInstance* inst, std::vector<WeaponInfoComponent>& list)
{
	u32 pos = (u32)event.GUIEvent.Caller->getID();
	WeaponInfoComponent wep = list[pos];
	if (wep.type != WEP_NONE) {
		list.erase(list.begin() + pos);
	}
	if (inst->weps[currentHardpoint].type != WEP_NONE) {
		list.push_back(inst->weps[currentHardpoint]);
	}
	inst->weps[currentHardpoint] = wep;

	std::string txt = stateController->weaponData[wep.wepDataId]->name;

	hardpoints[currentHardpoint]->setText(wstr(txt).c_str());
	if (wep.usesAmmunition) {
		reloadHardpoints[currentHardpoint]->setVisible(true);
	}
	else {
		reloadHardpoints[currentHardpoint]->setVisible(false);
	}
	clearWeaponList();
	return false;
}

bool GuiCampaignLoadoutMenu::physWepSelect(const SEvent& event, ShipInstance* inst, std::vector<WeaponInfoComponent>& list)
{
	u32 pos = (u32)event.GUIEvent.Caller->getID();
	WeaponInfoComponent wep = list[pos];
	if (wep.type != WEP_NONE) {
		list.erase(list.begin() + pos);
	}
	if (inst->physWep.type != WEP_NONE) {
		list.push_back(inst->physWep);
	}
	inst->physWep = wep;

	std::string txt = stateController->physWeaponData[wep.wepDataId]->name;

	physWepHardpoint->setText(wstr(txt).c_str());
	clearWeaponList();
	return false;
}

bool GuiCampaignLoadoutMenu::onWepSelect(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;

	return wepSelect(event, getCurShip(), stateController->campaign.availableWeapons);
}

bool GuiCampaignLoadoutMenu::onPhysWepSelect(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;

	return physWepSelect(event, getCurShip(), stateController->campaign.availablePhysWeapons);
}

bool GuiCampaignLoadoutMenu::onUseShip(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	stateController->campaign.player->assignedShip = getCurShip();
	getCurShip()->inUseBy = stateController->campaign.player;
	displayShip(getCurShip());
	/*
	ShipInstance inst = getCurShip();
	ShipInstance player = stateController->campaign.playerShip;
	ShipInstance& pos = getCurShip();
	stateController->campaign.playerShip = inst;
	pos = player;
	shipSelector.curPos = -1;
	displayShip(getCurShip());
	*/
	return false;
}

bool GuiCampaignLoadoutMenu::onReload(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;

	Campaign& camp = stateController->campaign;
	if (camp.totalAmmunition == 0) {
		guiController->setOkPopup("", "You don't have the ammunition to reload anything!");
		guiController->showOkPopup();
		return false;
	}
	u32 id = (u32)event.GUIEvent.Caller->getID();
	WeaponInfoComponent& wep = getCurShip()->weps[id];
	u32 clipsToReload = (wep.maxAmmunition - wep.ammunition) / wep.maxClip;
	if (clipsToReload == 0) {
		guiController->setOkPopup("", "This weapon is already full on ammo.");
		guiController->showOkPopup();
		return false;
	}
	if (camp.totalAmmunition < clipsToReload) {
		u32 reloadedAmmo = clipsToReload * wep.maxClip;
		std::string txt = "You don't have the ammo to reload this gun completely. A partial reload will have to do. \n \n Ammo reloaded: " + std::to_string(reloadedAmmo);
		wep.ammunition += reloadedAmmo;
		camp.totalAmmunition = 0;
		guiController->setOkPopup("", txt);
		guiController->showOkPopup();
		displayCarrierInfo();
		return false;
	}

	wep.ammunition = wep.maxAmmunition;
	camp.totalAmmunition -= clipsToReload;
	displayCarrierInfo();
	return false;
}
bool GuiCampaignLoadoutMenu::onRepair(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	Campaign& camp = stateController->campaign;
	if (camp.totalRepairCapacity == 0) {
		guiController->setOkPopup("", "You don't have enough resources to repair anything!");
		guiController->showOkPopup();
		return false;
	}
	ShipInstance* ship = getCurShip();
	f32 healthToRepair = ship->hp.maxHealth - ship->hp.health;
	if (healthToRepair == 0) {
		guiController->setOkPopup("", "This ship is already fully repaired.");
		guiController->showOkPopup();
		return false;
	}
	if (camp.totalRepairCapacity < healthToRepair) {
		std::string txt = "You don't have the resources to repair this ship completely. A partial repair will have to be enough. \n \n Damage repaired: " + fprecis(camp.totalRepairCapacity, 5);
		ship->hp.health += camp.totalRepairCapacity;
		camp.totalRepairCapacity = 0;
		guiController->setOkPopup("", txt);
		guiController->showOkPopup();
		displayCarrierInfo();
		return false;
	}
	camp.totalRepairCapacity -= healthToRepair;
	ship->hp.health = ship->hp.maxHealth;
	displayShip(ship);
	displayCarrierInfo();
	return false;
}