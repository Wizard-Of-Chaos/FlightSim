#include "GuiWingmanMenu.h"
#include "GuiController.h"
#include "GameStateController.h"

void GuiWingmanMenu::init()
{
	dimension2du buttonSize(160, 40);
	dimension2du pairButtonSize(100, 40);
	backButton = guienv->addButton(rect<s32>(position2di(0, 0), buttonSize), root, -1, L"Back", L"Return to campaign menu.");
	setHybridButton(backButton);
	menuDisplay = guienv->addImage(rect<s32>(position2di(362, 120), dimension2du(237, 319)), root);
	menuDisplay->setImage(driver->getTexture("ui/freestandingmenu.png"));
	scaleAlign(menuDisplay);
	guiController->setCallback(backButton, std::bind(&GuiWingmanMenu::backToMenu, this, std::placeholders::_1));

	for (u32 i = 0; i < 3; ++i) {
		wingButtons[i].wingman = guienv->addButton(rect<s32>(position2di(80, 150 + i * 100), pairButtonSize), root, (s32)i, L"None", L"Select a wingman.");
		wingButtons[i].ship = guienv->addButton(rect<s32>(position2di(185, 150 + i * 100), pairButtonSize), root, (s32)i, L"None", L"Select the ship for this wingman.");
		setHoloButton(wingButtons[i].wingman, true);
		setHoloButton(wingButtons[i].ship, true);
		guiController->setCallback(wingButtons[i].wingman, std::bind(&GuiWingmanMenu::onWingmanChange, this, std::placeholders::_1));
		guiController->setCallback(wingButtons[i].ship, std::bind(&GuiWingmanMenu::onShipChange, this, std::placeholders::_1));

	}

	description = guienv->addStaticText(L"DESCRIPTION\n\n Description will go here", rect<s32>(position2di(610, 120), dimension2du(340, 320)), true, true, root);
	setUIText(description);
	hide();
}

bool GuiWingmanMenu::backToMenu(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	//check to make sure all wingmen here have an assigned ship
	for (u32 i = 0; i < 3; ++i) {
		if (stateController->campaign.assignedWingmen[i] && !stateController->campaign.assignedShips[i]) {
			guiController->setOkPopup(
				"Missing Ship",
				"One or more of your wingmen doesn't have a ship selected! You can't just hurl them into space!",
				"Ugh, fine"
			);
			guiController->showOkPopup();
			return false;
		}
	}
	guiController->setActiveDialog(GUI_CAMPAIGN_MENU);
	return false;
}

void GuiWingmanMenu::show()
{
	displayWingStatus();
	root->setRelativePosition(rect<s32>(position2di(0, 0), driver->getScreenSize()));
	root->setVisible(true);
};

bool GuiWingmanMenu::onWingmanChange(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	currentSlot = event.GUIEvent.Caller->getID();
	displayWingmen();
	return false;
}
bool GuiWingmanMenu::onWingmanSelect(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	s32 id = event.GUIEvent.Caller->getID();
	clearDisplayedList();
	if (id == -1) {
		if (stateController->campaign.assignedWingmen[currentSlot]) {
			stateController->campaign.assignedWingmen[currentSlot]->assigned = false;
			stateController->campaign.assignedWingmen[currentSlot] = nullptr;
			stateController->campaign.assignedShips[currentSlot] = nullptr;
		}
		displayWingStatus();
		return false;
	}
	if (stateController->campaign.assignedWingmen[currentSlot]) {
		stateController->campaign.assignedWingmen[currentSlot]->assigned = false;
		stateController->campaign.assignedShips[currentSlot] = nullptr;
	}
	for (WingmanData* dat : stateController->campaign.wingmen) {
		if (dat->id == (u32)id) {
			stateController->campaign.assignedWingmen[currentSlot] = dat;
			if (dat->assignedShip) {
				stateController->campaign.assignedShips[currentSlot] = dat->assignedShip;
			}
			dat->assigned = true;
			break;
		}
	}
	displayWingStatus();
	return false;
}
bool GuiWingmanMenu::onWingmanHover(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_ELEMENT_HOVERED) return true;
	return false;
}

bool GuiWingmanMenu::onShipChange(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	currentSlot = event.GUIEvent.Caller->getID();
	if (!stateController->campaign.assignedWingmen[currentSlot]) {
		guiController->setOkPopup("No Wingman Selected", 
			"You can't select a ship for a nonexistent wingman!", 
			"Oh, right");
		guiController->showOkPopup();
		return false;
	}
	displayShips();
	return false;
}
bool GuiWingmanMenu::onShipSelect(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	clearDisplayedList();
	s32 id = event.GUIEvent.Caller->getID();
	if (id < 0) {
		auto assigned = stateController->campaign.assignedShips[currentSlot];
		if (assigned) {
			if (assigned->inUseBy) {
				assigned->inUseBy->assignedShip = nullptr;
				assigned->inUseBy = nullptr;
			}
		}
		displayWingStatus();
		return false;
	}
	else {
		for (auto inst : stateController->campaign.ships) {
			if (inst->id == (u32)id) {
				stateController->campaign.assignedShips[currentSlot] = inst;
				inst->inUseBy = stateController->campaign.assignedWingmen[currentSlot];
				inst->inUseBy->assignedShip = inst;
				break;
			}
		}
	}
	displayWingStatus();
	return false;
}
bool GuiWingmanMenu::onShipHover(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_ELEMENT_HOVERED) return true;
	return false;
}

void GuiWingmanMenu::displayWingmen()
{
	f32 screenRatioHoriz = 180.f / 960.f;
	f32 screenRatioVert = 25.f / 540.f;
	u32 width = (u32)(screenRatioHoriz * root->getAbsolutePosition().getWidth());
	u32 height = (u32)(screenRatioVert * root->getAbsolutePosition().getHeight());
	dimension2du buttonSize(width, height);
	s32 num = 0;

	u32 horizPos = (u32)(30.f / 960.f * root->getAbsolutePosition().getWidth());
	u32 vertPos = (u32)(30.f / 540.f * root->getAbsolutePosition().getHeight());
	for (WingmanData* man : stateController->campaign.wingmen) {
		if (man->assigned) continue;
		IGUIButton* newButton = guienv->addButton(rect<s32>(position2di(horizPos, vertPos + num * vertPos), buttonSize), menuDisplay, 
			man->id, wstr(man->name).c_str(), L"Select this wingman.");
		setHoloButton(newButton);
		guiController->setCallback(newButton, std::bind(&GuiWingmanMenu::onWingmanSelect, this, std::placeholders::_1));
		buttonList.push_back(newButton);
		++num;
	}
	IGUIButton* newButton = guienv->addButton(rect<s32>(position2di(horizPos, vertPos + num * vertPos), buttonSize), menuDisplay,
		-1, L"None", L"De-select this wingman.");
	setHoloButton(newButton);
	guiController->setCallback(newButton, std::bind(&GuiWingmanMenu::onWingmanSelect, this, std::placeholders::_1));
	buttonList.push_back(newButton);
}

void GuiWingmanMenu::displayShips()
{
	f32 screenRatioHoriz = 180.f / 960.f;
	f32 screenRatioVert = 25.f / 540.f;
	u32 width = (u32)(screenRatioHoriz * root->getAbsolutePosition().getWidth());
	u32 height = (u32)(screenRatioVert * root->getAbsolutePosition().getHeight());
	dimension2du buttonSize(width, height);
	s32 num = 0;

	u32 horizPos = (u32)(30.f / 960.f * root->getAbsolutePosition().getWidth());
	u32 vertPos = (u32)(30.f / 540.f * root->getAbsolutePosition().getHeight());
	for (auto inst : stateController->campaign.ships) {
		if (inst->inUseBy) continue;
		std::wstring name = wstr(stateController->shipData[inst->ship.shipDataId]->name);
		IGUIButton* newButton = guienv->addButton(rect<s32>(position2di(horizPos, vertPos + num * vertPos), buttonSize), menuDisplay,
			(s32)inst->id, name.c_str(), L"Select this ship.");
		++num;
		setHoloButton(newButton);
		guiController->setCallback(newButton, std::bind(&GuiWingmanMenu::onShipSelect, this, std::placeholders::_1));
		buttonList.push_back(newButton);
	}
	IGUIButton* newButton = guienv->addButton(rect<s32>(position2di(horizPos, vertPos + num * vertPos), buttonSize), menuDisplay,
		-1, L"None", L"De-select this ship.");
	setHoloButton(newButton);
	guiController->setCallback(newButton, std::bind(&GuiWingmanMenu::onShipSelect, this, std::placeholders::_1));
	buttonList.push_back(newButton);
}

void GuiWingmanMenu::clearDisplayedList()
{
	for (IGUIButton* button : buttonList) {
		guiController->removeCallback(button);
		button->remove();
	}
	buttonList.clear();
}

void GuiWingmanMenu::displayWingStatus()
{
	for (u32 i = 0; i < 3; ++i) {
		if (stateController->campaign.assignedWingmen[i]) {
			wingButtons[i].wingman->setText(wstr(stateController->campaign.assignedWingmen[i]->name).c_str());
		}
		else {
			wingButtons[i].wingman->setText(L"None");
		}
		if (stateController->campaign.assignedShips[i]) {
			auto inst = stateController->campaign.assignedShips[i];
			std::wstring name = wstr(stateController->shipData[inst->ship.shipDataId]->name);
			wingButtons[i].ship->setText(name.c_str());
		}
		else {
			wingButtons[i].ship->setText(L"None");
		}
	}
}