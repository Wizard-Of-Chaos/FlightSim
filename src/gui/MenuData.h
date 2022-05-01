#pragma once

#ifndef MENUDATA_H
#define MENUDATA_H
#include <map>

#include "BaseHeader.h"
#include "GuiDialog.h"
#include "GuiMainMenu.h"
#include "GuiPauseMenu.h"
#include "GuiDeathMenu.h"
#include "GuiSettingsMenu.h"
#include "GuiLoadoutMenu.h"
#include "GuiCampaignMenu.h"

//Enum for the different types of dialog. Add to this enum when implementing a new GuiDialog.
//This allows the GuiController to be able to set the active dialog accordingly.
enum MENU_TYPE {
	GUI_MAIN_MENU = 1,
	GUI_PAUSE_MENU = 2,
	GUI_DEATH_MENU = 3,
	GUI_SETTINGS_MENU = 4,
	GUI_LOADOUT_MENU = 5,
	GUI_CAMPAIGN_MENU = 6,
};
#endif