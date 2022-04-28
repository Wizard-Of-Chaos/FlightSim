#include "GuiDeathMenu.h"
#include "GuiController.h"
#include "GameStateController.h"
#include <time.h>

void GuiDeathMenu::init()
{
	if (root) root->remove();
	root = guienv->addStaticText(L"", rect<s32>(position2di(0, 0), baseSize));

	u32 verticalSlice = baseSize.Height / 6;
	u32 horizontalPos = baseSize.Width / 3;
	dimension2du buttonSize(horizontalPos, verticalSlice);

	returnToCampaign = guienv->addButton(rect<s32>(position2di(horizontalPos, verticalSlice + 8), buttonSize), root, -1, L"Return To Ship", L"Repairs are for winners.");
	returnToMenu = guienv->addButton(rect<s32>(position2di(horizontalPos, verticalSlice * 2 + 8 * 2), buttonSize), root, DEATHMENU_RETURN, L"Main Menu", L"You'll be back.");
	taunt = guienv->addStaticText(guiController->getTaunt().c_str(), rect<s32>(position2di(horizontalPos, 32), buttonSize), false, true, root);

	setUIText(taunt);
	setMetalButton(returnToMenu);
	setMetalButton(returnToCampaign);

	guiController->setCallback(returnToMenu, std::bind(&GuiDeathMenu::onReturnMenu, this, std::placeholders::_1));
	guiController->setCallback(returnToCampaign, std::bind(&GuiDeathMenu::onReturnCampaign, this, std::placeholders::_1));

	hide();
}

void GuiDeathMenu::show()
{
	if (gameController->isPlayerAlive) {
		taunt->setText(guiController->getCongrats().c_str());
	}
	else {
		taunt->setText(guiController->getTaunt().c_str());
	}
	root->setRelativePosition(rect<s32>(position2di(0, 0), driver->getScreenSize()));
	root->setVisible(true);
}

bool GuiDeathMenu::onReturnCampaign(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED || !gameController->isPlayerAlive) return true;
	stateController->backToCampaign();
	return false;
}

bool GuiDeathMenu::onReturnMenu(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;

	stateController->setState(GAME_MENUS);
	return false;
}