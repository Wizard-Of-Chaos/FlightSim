#pragma once

#ifndef GUIMAINMENU_H
#define GUIMAINMENU_H

#include "BaseHeader.h"
#include "GuiDialog.h"

//forward declarations of things needed
class GameStateController;
class GuiController;

//enum to handle all the different button IDs for this dialog.
enum MAIN_MENU_BUTTONS
{
	MAINMENU_START,
	MAINMENU_SETTINGS,
	MAINMENU_QUIT
};

/*
* The main menu dialog. Includes the buttons and logic to handle those buttons and start the game.
*/
class GuiMainMenu : public GuiDialog
{
	public:
		GuiMainMenu(GuiController* controller) : GuiDialog(controller), startGame(0), settings(0), quitGame(0) {}

		virtual void init();

		bool onStart(const SEvent& event);
		bool onSettings(const SEvent& event);
		bool onQuit(const SEvent& event);
	private:
		IGUIButton* startGame;
		IGUIButton* settings;
		IGUIButton* quitGame;
};


#endif
