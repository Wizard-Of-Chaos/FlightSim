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

	//Menus should set up their root node. Who knows, maybe it could be a cool background.
	//root = guiController->guienv->addStaticText(L"MAIN", rect<s32>(position2di(0, 0), baseSize));
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

	hide();
}

void GuiMainMenu::handleEvent(const SEvent& event)
{
	switch (event.EventType)
	{
		case EET_MOUSE_INPUT_EVENT:
			break;
		case EET_KEY_INPUT_EVENT:
			break;
		case EET_GUI_EVENT:
			s32 id = event.GUIEvent.Caller->getID();
			if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) { //important: don't forget this if statement on buttons, otherwise all of 'em will try and activate at once.
				switch (id) {
				case MAINMENU_START:
					guiController->stateController->setState(GAME_RUNNING);
					break;
				case MAINMENU_SETTINGS:
					break;
				case MAINMENU_QUIT:
					guiController->device->closeDevice();
					break;
				}
			}
			break;
	}
}