#pragma once

#ifndef GUIMAINMENU_H
#define GUIMAINMENU_H

#include "BaseHeader.h"
#include "GuiDialog.h"

//enum to handle all the different button IDs for this dialog.
enum MAIN_MENU_BUTTONS
{
	MAINMENU_START,
	MAINMENU_LOADOUT,
	MAINMENU_SETTINGS,
	MAINMENU_QUIT
};

/*
* The main menu dialog. Includes the buttons and logic to handle those buttons and start the game.
*/
class GuiMainMenu : public GuiDialog
{
	public:
		GuiMainMenu() : GuiDialog(), startGame(0), settings(0), quitGame(0) {}

		virtual void init();
		virtual void show();

		bool onStart(const SEvent& event);
		bool onLoadout(const SEvent& event);
		bool onSettings(const SEvent& event);
		bool onQuit(const SEvent& event);

		bool onShow(f32 dt);
		bool onHide(f32 dt);
	private:
		bool initPopupUsed = true;
		f32 showAnimTimer;
		IGUIImage* screen;
		IGUIButton* startGame;
		IGUIButton* loadout;
		IGUIButton* settings;
		IGUIButton* quitGame;
};


#endif
