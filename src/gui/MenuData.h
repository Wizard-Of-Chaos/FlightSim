#pragma once

#ifndef MENUDATA_H
#define MENUDATA_H
#include <map>

#include "BaseHeader.h"
#include "GuiDialog.h"
#include "GuiMainMenu.h"
#include "GuiPauseMenu.h"
#include "GuiDeathMenu.h"

//Enum for the different types of dialog. Add to this enum when implementing a new GuiDialog.
//This allows the GuiController to be able to set the active dialog accordingly.
enum MENU_TYPE {
	GUI_MAIN_MENU = 1,
	GUI_PAUSE_MENU = 2,
	GUI_DEATH_MENU = 3
};

//Holds the data for all the menus and maps them to the appropriate dialogs. Used in GuiController.
struct MenuData
{
	std::map<MENU_TYPE, GuiDialog*> menuDialogs;
};

#endif