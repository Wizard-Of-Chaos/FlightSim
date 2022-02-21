#include "GuiController.h"
#include "GameStateController.h"
#include <iostream>

GuiController::GuiController(GameStateController* controller)
{
	stateController = controller;
	device = controller->device;
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	soundEngine = controller->soundEngine;
	guienv = device->getGUIEnvironment();
	activeDialog = 0;
}

void GuiController::init()
{
	//All menus get initialized here. Don't delete them.
	//If you've just added a new menu, initialize it here.
	menus.menuDialogs[GUI_MAIN_MENU] = new GuiMainMenu(this);
	menus.menuDialogs[GUI_MAIN_MENU]->init();
	menus.menuDialogs[GUI_PAUSE_MENU] = new GuiPauseMenu(this);
	menus.menuDialogs[GUI_PAUSE_MENU]->init();
	menus.menuDialogs[GUI_DEATH_MENU] = new GuiDeathMenu(this);
	menus.menuDialogs[GUI_DEATH_MENU]->init();
	//set up death menu

	activeDialog = menus.menuDialogs[GUI_MAIN_MENU];
	activeDialog->show();
	//default main menu
}

void GuiController::close()
{
	if (activeDialog) {
		activeDialog->hide();
		activeDialog = nullptr;
	}
}

bool GuiController::OnEvent(const SEvent& event)
{
	//Hurls the event to the active dialog.
	if (activeDialog) {
		activeDialog->handleEvent(event);
	}
	return false;
}

void GuiController::update()
{
	//...I'm not actually sure anything needs to be done, but if it does, it gets done here.
}

//If you've just added a new menu, go make sure that you added it as a menu type in MenuData.h
void GuiController::setActiveDialog(MENU_TYPE menu)
{
	if (activeDialog) {
		activeDialog->hide();
	}
	activeDialog = menus.menuDialogs[menu];
	activeDialog->show();
	guienv->getRootGUIElement()->bringToFront(activeDialog->getRoot());
}