#include "GuiWingmanMenu.h"
#include "GuiController.h"

void GuiWingmanMenu::init()
{
	dimension2du buttonSize(160, 40);
	backButton = guienv->addButton(rect<s32>(position2di(0, 0), buttonSize), root, -1, L"Back", L"Return to campaign menu.");
	setHybridButton(backButton);
	guiController->setCallback(backButton, std::bind(&GuiWingmanMenu::backToMenu, this, std::placeholders::_1));
	hide();
}

bool GuiWingmanMenu::backToMenu(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	guiController->setActiveDialog(GUI_CAMPAIGN_MENU);
	return false;
}