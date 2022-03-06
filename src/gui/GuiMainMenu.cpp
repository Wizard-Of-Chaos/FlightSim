#include "GuiMainMenu.h"
#include "GuiController.h"
#include "GameStateController.h"
#include <iostream>

void GuiMainMenu::init()
{
	//Convenience declarations here to determine how big a button should be.
	//In this case, it's setting up some fairly large buttons straight down the middle of the screen.
	u32 verticalSlice = baseSize.Height / 6;
	u32 horizontalPos = baseSize.Width / 3;
	dimension2du buttonSize(horizontalPos, verticalSlice); //third of the screen size and a sixth of the height

	//All buttons have the root node set as the parent. This allows a single call to root->setVisible in order to display or hide the menu.
	startGame = guiController->guienv->addButton(rect<s32>(position2di(horizontalPos, 32), buttonSize), root, MAINMENU_START, L"Start Game", L"Are you prepared to shoot rocks?");
	settings = guiController->guienv->addButton(rect<s32>(position2di(horizontalPos, 32 * 2 + verticalSlice), buttonSize), root, MAINMENU_SETTINGS, L"Settings", L"Like we have any worthwhile settings.");
	quitGame = guiController->guienv->addButton(rect<s32>(position2di(horizontalPos, 32 * 3 + (verticalSlice * 2)), buttonSize), root, MAINMENU_QUIT, L"Quit Game", L"You'll be back.");
	scaleAlign(startGame);
	scaleAlign(settings);
	scaleAlign(quitGame);
	startGame->setScaleImage(true);
	settings->setScaleImage(true);
	quitGame->setScaleImage(true);

	GuiCallback start = std::bind(&GuiMainMenu::onStart, this, std::placeholders::_1);
	GuiCallback set = std::bind(&GuiMainMenu::onSettings, this, std::placeholders::_1);
	GuiCallback quit = std::bind(&GuiMainMenu::onQuit, this, std::placeholders::_1);
	guiController->setCallback(startGame, start);
	guiController->setCallback(settings, set);
	guiController->setCallback(quitGame, quit);
	hide();
}

bool GuiMainMenu::onStart(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;

	guiController->stateController->setState(GAME_RUNNING);
	return false;

}
bool GuiMainMenu::onSettings(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true; 

	guiController->setActiveDialog(GUI_SETTINGS_MENU);
	return false;
}
bool GuiMainMenu::onQuit(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;

	guiController->device->closeDevice();
	return false;
}