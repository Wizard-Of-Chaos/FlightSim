#pragma once
#ifndef GUILOADOUTMENU_H
#define GUILOADOUTMENU_H
#include "BaseHeader.h"
#include "GuiDialog.h"
#include "LoadoutData.h"

class GuiController;
class GameStateController;

//This header is a doozy.

//Enum containing all the buttons available on the loadout menu (probably unnecessary, but just in case).
enum LOADOUT_MENU_BUTTON
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

	LOADOUTMENU_PHYSWEP,
	LOADOUTMENU_PHYSWEP_L,
	LOADOUTMENU_PHYSWEP_R,

	LOADOUTMENU_RETURN_TO_MAIN,
	LOADOUTMENU_SHIP_DESC,
	LOADOUTMENU_WEP_DESC
};

/*
* ButtonPairs are used on the loadout menu in order to be able to display several things : the name of the weapon
* on the hardpoint associated with them and a button switching left and right to enable the player to flip cleanly
* through their weapon options. The weapon data is loaded from the GameStateController map for weapons.
*/
struct ButtonPair
{
	IGUIStaticText* name;
	IGUIButton* buttonL;
	IGUIButton* buttonR;
};

/*
* The dialog for the loadout menu. Despite what the code looks like, it IS self explanatory!
* Essentially it just contains all of the buttons and text required to display things on the loadout menu.
*/
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

		bool onPhysWeaponChangeRight(const SEvent& event);
		bool onPhysWeaponChangeLeft(const SEvent& event);
		bool onPhysWeaponHover(const SEvent& event);

	private:
		/*
		* This function requires a bit of explaining. It will set up a button pair at the given position.
		* Inputs:
		* num - The number slot the button pair corresponds to. If it's -1, this will be ignored. If you want to create
		* a button pair corresponding to hardpoint 4 on the ship, you would pass in 3.
		* pos - The position on the screen. This is calculated during the setup as to where the buttons get placed.
		* left - a u32 value corresponding to the enum for the ID. LOADOUTMENU_WEAPON_4_L is 13.
		* right - a u32 value corresponding to the enum for the ID. LOADOUTMENU_WEAPON_4_R is 14.
		* cent - a u32 value corresponding to the enum for the ID. LOADOUTMENU_WEAPON_4 is 12.
		* 
		* Note that this will NOT set the callbacks for the buttons; you can apply callbacks on the button pair returned.
		*/
		ButtonPair createButtonPair(u32 num, position2di pos, u32 left, u32 right, u32 cent);
		void loadoutToWString();
		void setShipNameAndDesc(u32 shipId);

		ButtonPair weaponButtons[MAX_HARDPOINTS];
		ButtonPair physWeaponButtons;
		ButtonPair shipButtons;

		IGUIStaticText* shipDescription;
		IGUIStaticText* wepDescription;
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