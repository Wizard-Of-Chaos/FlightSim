#include "GuiWingmanMenu.h"
#include "GuiController.h"

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
		wingButtons[i].wingman = guienv->addButton(rect<s32>(position2di(80, 150 + i * 100), pairButtonSize), root, -1, L"None", L"Select a wingman.");
		wingButtons[i].ship = guienv->addButton(rect<s32>(position2di(185, 150 + i * 100), pairButtonSize), root, -1, L"None", L"Select the ship for this wingman.");
		setHoloButton(wingButtons[i].wingman, true);
		setHoloButton(wingButtons[i].ship, true);
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