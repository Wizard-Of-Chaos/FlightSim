#pragma once
#ifndef GUILOADOUTMENU_H
#define GUILOADOUTMENU_H
#include "BaseHeader.h"
#include "GuiDialog.h"
#include "LoadoutData.h"

class GuiController;
class GameStateController;

enum LOADOUT_MENU_BUTTONS
{
	LOADOUTMENU_WEAPON_0,
	LOADOUTMENU_WEAPON_1,
	LOADOUTMENU_WEAPON_2,
	LOADOUTMENU_WEAPON_3,
	LOADOUTMENU_WEAPON_4,
	LOADOUTMENU_WEAPON_5,
	LOADOUTMENU_WEAPON_6,
	LOADOUTMENU_WEAPON_7,
	LOADOUTMENU_RETURN_TO_MAIN,
	LOADOUTMENU_SHIP_SELECT
};

class GuiLoadoutMenu : public GuiDialog
{
	public:
		GuiLoadoutMenu(GuiController* controller) : GuiDialog(controller) {}
		virtual void init();
		bool onShipSelect(const SEvent& event);
		bool onWeaponSelect(const SEvent& event);
		bool onReturn(const SEvent& event);

	private:
		IGUIButton* weapon0;
		IGUIButton* weapon1;
		IGUIButton* weapon2;
		IGUIButton* weapon3;
		IGUIButton* weapon4;
		IGUIButton* weapon5;
		IGUIButton* weapon6;
		IGUIButton* weapon7;
		IGUIButton* shipSelect;
		IGUIStaticText* shipDescription;
		IGUIButton* returnToMain;
};
#endif 