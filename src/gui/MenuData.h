#pragma once

#ifndef MENUDATA_H
#define MENUDATA_H
#include <map>

#include "BaseHeader.h"
#include "GuiDialog.h"
#include "GuiMainMenu.h"
#include "GuiPauseMenu.h"
#include "GuiDeathMenu.h"

//Include and add menu types here.
enum MENU_TYPE {
	GUI_MAIN_MENU = 1,
	GUI_PAUSE_MENU = 2,
	GUI_DEATH_MENU = 3
};

struct MenuData
{
	std::map<MENU_TYPE, GuiDialog*> menuDialogs;
};

#endif