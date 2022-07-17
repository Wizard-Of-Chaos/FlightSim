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
	guiController->setActiveDialog(GUI_CAMPAIGN_MENU);
	return false;
}

void GuiWingmanMenu::show()
{
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
	clearDisplayedList();
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
	displayShips();
	return false;
}
bool GuiWingmanMenu::onShipSelect(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	clearDisplayedList();
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
	}
}

void GuiWingmanMenu::displayShips()
{

}

void GuiWingmanMenu::clearDisplayedList()
{
	for (IGUIButton* button : buttonList) {
		guiController->removeCallback(button);
		button->remove();
	}
	buttonList.clear();
}