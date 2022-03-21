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
	LOADOUTMENU_WEAPON_0_L,
	LOADOUTMENU_WEAPON_0_R,
	LOADOUTMENU_WEAPON_1,
	LOADOUTMENU_WEAPON_1_L,
	LOADOUTMENU_WEAPON_1_R,
	LOADOUTMENU_WEAPON_2,
	LOADOUTMENU_WEAPON_2_L,
	LOADOUTMENU_WEAPON_2_R,
	LOADOUTMENU_WEAPON_3,
	LOADOUTMENU_WEAPON_3_L,
	LOADOUTMENU_WEAPON_3_R,
	LOADOUTMENU_WEAPON_4,
	LOADOUTMENU_WEAPON_4_L,
	LOADOUTMENU_WEAPON_4_R,
	LOADOUTMENU_WEAPON_5,
	LOADOUTMENU_WEAPON_5_L,
	LOADOUTMENU_WEAPON_5_R,
	LOADOUTMENU_WEAPON_6,
	LOADOUTMENU_WEAPON_6_L,
	LOADOUTMENU_WEAPON_6_R,
	LOADOUTMENU_WEAPON_7,
	LOADOUTMENU_WEAPON_7_L,
	LOADOUTMENU_WEAPON_7_R,
	LOADOUTMENU_SHIP_SELECT,
	LOADOUTMENU_SHIP_SELECT_L,
	LOADOUTMENU_SHIP_SELECT_R,

	LOADOUTMENU_RETURN_TO_MAIN,
	LOADOUTMENU_SHIP_DESC,
	LOADOUTMENU_WEP_DESC
};

struct ButtonPair
{
	IGUIStaticText* name;
	IGUIButton* buttonL;
	IGUIButton* buttonR;
};

class GuiLoadoutMenu : public GuiDialog
{
	public:
		GuiLoadoutMenu(GuiController* controller) : GuiDialog(controller) {}
		virtual void init();
		bool onShipChangeRight(const SEvent& event);
		bool onShipChangeLeft(const SEvent& event);
		bool onWeaponChangeRight(const SEvent& event);
		bool onWeaponChangeLeft(const SEvent& event);
		bool onWeaponHover(const SEvent& event);
		bool onReturn(const SEvent& event);

	private:
		void createButtonPair(u32 num);
		void loadoutToWString();
		void setShipNameAndDesc(u32 shipId);

		ButtonPair weaponbuttons[MAX_HARDPOINTS];

		IGUIStaticText* ship;
		IGUIStaticText* shipDescription;
		IGUIStaticText* wepDescription;
		IGUIButton* shipL;
		IGUIButton* shipR;
		IGUIButton* returnToMain;

		u32 buttonVert;
		u32 buttonHoriz;
		u32 textVert;
		u32 textHoriz;
		dimension2du buttonSize;
		dimension2du textSize;
		u32 buf;

		std::wstring name;
		std::wstring desc;
		std::wstring weps[MAX_HARDPOINTS];
};
#endif 