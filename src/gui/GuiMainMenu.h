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
	GUI_START_GAME,
	GUI_SETTINGS,
	GUI_QUIT_GAME
};

class GuiMainMenu : public GuiDialog
{
	public:
		GuiMainMenu(GuiController* controller) : GuiDialog(controller), startGame(0), settings(0), quitGame(0) {}

		virtual void init();

		virtual void handleEvent(const SEvent& event);
	private:
		IGUIButton* startGame;
		IGUIButton* settings;
		IGUIButton* quitGame;
};


#endif
