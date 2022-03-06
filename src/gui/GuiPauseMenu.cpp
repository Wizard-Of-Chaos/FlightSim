#include "GuiPauseMenu.h"
#include "GuiController.h"
#include "GameStateController.h"

void GuiPauseMenu::init()
{
	dimension2du screenSize = guiController->driver->getScreenSize();
	u32 verticalSlice = baseSize.Height / 6;
	u32 horizontalPos = baseSize.Width / 3;
	dimension2du buttonSize(horizontalPos, verticalSlice);

	resumeGame = guiController->guienv->addButton(rect<s32>(position2di(horizontalPos, 32), buttonSize), root, PAUSEMENU_RESUME, L"Resume Game", L"Get back in there!");
	pauseSettings = guiController->guienv->addButton(rect<s32>(position2di(horizontalPos, 32 * 2 + verticalSlice), buttonSize), root, PAUSEMENU_SETTINGS, L"Settings", L"What, is your sensitivity too low?");
	exitToMenus = guiController->guienv->addButton(rect<s32>(position2di(horizontalPos, 32 * 3 + verticalSlice * 2), buttonSize), root, PAUSEMENU_EXIT, L"Exit to Main Menu", L"Run, coward!");
	scaleAlign(resumeGame);
	scaleAlign(pauseSettings);
	scaleAlign(exitToMenus);

	resumeGame->setScaleImage(true);
	pauseSettings->setScaleImage(true);
	exitToMenus->setScaleImage(true);

	guiController->setCallback(resumeGame, std::bind(&GuiPauseMenu::onResume, this, std::placeholders::_1));
	guiController->setCallback(pauseSettings, std::bind(&GuiPauseMenu::onSettings, this, std::placeholders::_1));
	guiController->setCallback(exitToMenus, std::bind(&GuiPauseMenu::onExit, this, std::placeholders::_1));

	hide();
}

bool GuiPauseMenu::onResume(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;

	guiController->stateController->setState(GAME_RUNNING);
	return false;
}
bool GuiPauseMenu::onSettings(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	return false;
}
bool GuiPauseMenu::onExit(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;

	guiController->stateController->setState(GAME_MENUS);
	return false;
}