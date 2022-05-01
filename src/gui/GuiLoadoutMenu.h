#pragma once
#ifndef GUILOADOUTMENU_H
#define GUILOADOUTMENU_H
#include "BaseHeader.h"
#include "GuiDialog.h"
#include "LoadoutData.h"

class GuiController;
class GameStateController;

//This header is a doozy.
/*
* ButtonPairs are used on the loadout menu in order to be able to display several things : the name of the weapon
* on the hardpoint associated with them and a button switching left and right to enable the player to flip cleanly
* through their weapon options. The weapon data is loaded from the GameStateController map for weapons.
*/
struct ButtonPair
{
	IGUIStaticText* name;
	IGUIButton* left;
	IGUIButton* right;
};

/*
* The dialog for the loadout menu. Despite what the code looks like, it IS self explanatory!
* Essentially it just contains all of the buttons and text required to display things on the loadout menu.
*/
class GuiLoadoutMenu : public GuiDialog
{
	public:
		GuiLoadoutMenu() : GuiDialog() {}
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

		virtual void show();

	private:
		/*
		* This function requires a bit of explaining. It will set up a button pair at the given position.
		* Inputs:
		* num - The number slot the button pair corresponds to. If it's -1, this will be ignored. If you want to create
		* a button pair corresponding to hardpoint 4 on the ship, you would pass in 3.
		* pos - The position on the screen. This is calculated during the setup as to where the buttons get placed.
		* 
		* Note that this will NOT set the callbacks for the buttons; you can apply callbacks on the button pair returned.
		*/
		ButtonPair createButtonPair(u32 num, position2di pos);
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
};
#endif 