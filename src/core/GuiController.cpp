#include "GuiController.h"
#include "GameStateController.h"
#include <iostream>

GuiController::GuiController()
{
	activeDialog = 0;

	gvReader tauntReader;
	tauntReader.read("attributes/taunts.txt");
	for (std::string line : tauntReader.lines) {
		taunts.push_back(std::wstring(line.begin(), line.end()));
	}
}

void GuiController::init()
{
	//All menus get initialized here. Don't delete them.
	//If you've just added a new menu, initialize it here.
	menus.menuDialogs[GUI_MAIN_MENU] = new GuiMainMenu;
	menus.menuDialogs[GUI_MAIN_MENU]->init();
	menus.menuDialogs[GUI_PAUSE_MENU] = new GuiPauseMenu;
	menus.menuDialogs[GUI_PAUSE_MENU]->init();
	menus.menuDialogs[GUI_DEATH_MENU] = new GuiDeathMenu;
	menus.menuDialogs[GUI_DEATH_MENU]->init();
	menus.menuDialogs[GUI_SETTINGS_MENU] = new GuiSettingsMenu;
	menus.menuDialogs[GUI_SETTINGS_MENU]->init();
	menus.menuDialogs[GUI_LOADOUT_MENU] = new GuiLoadoutMenu;
	menus.menuDialogs[GUI_LOADOUT_MENU]->init();
	//set up death menu

	activeDialog = menus.menuDialogs[GUI_MAIN_MENU];
	activeDialog->show();
	//default main menu
}

std::wstring GuiController::getTaunt()
{
	return taunts[std::rand() % taunts.size()]; //Pulls out a random taunt to mess with the player
}

void GuiController::close()
{
	if (activeDialog) {
		activeDialog->hide();
		activeDialog = nullptr;
	} //Doesn't actually delete anything; that's what menuCleanup is for (and guienv->clear() is called elsewhere to actually remove elements)
}

bool GuiController::OnEvent(const SEvent& event)
{
	//Hurls the event to the active dialog.
	if (activeDialog && event.EventType == EET_GUI_EVENT && callbacks.find(event.GUIEvent.Caller) != callbacks.end()) {
		return callbacks[event.GUIEvent.Caller](event);
	}
	return true;
}

void GuiController::setCallback(IGUIElement* elem, GuiCallback callback)
{
	callbacks[elem] = callback;
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