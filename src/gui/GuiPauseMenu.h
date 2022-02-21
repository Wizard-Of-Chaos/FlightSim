#pragma once

#ifndef GUIPAUSEMENU_H
#define GUIPAUSEMENU_H
#include "BaseHeader.h"
#include "GuiDialog.h"

class GuiController;
class GameStateController;

enum PAUSE_MENU_BUTTONS
{
	PAUSEMENU_RESUME,
	PAUSEMENU_SETTINGS,
	PAUSEMENU_EXIT
};

class GuiPauseMenu : public GuiDialog
{
	public:
		GuiPauseMenu(GuiController* controller) : GuiDialog(controller), resumeGame(0), pauseSettings(0), exitToMenus(0) {}
		~GuiPauseMenu() {}

		virtual void init();
		virtual void handleEvent(const SEvent& event);
	private:
		IGUIButton* resumeGame;
		IGUIButton* pauseSettings;
		IGUIButton* exitToMenus;

};

#endif