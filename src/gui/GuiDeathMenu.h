#pragma once
#ifndef GUIDEATHMENU_H
#define GUIDEATHMENU_H
#include "BaseHeader.h"
#include "GuiDialog.h"
#include "GvReader.h"
#include <random>

enum DEATH_MENU_BUTTONS
{
	DEATHMENU_RETURN
};

class GuiDeathMenu : public GuiDialog
{
	public:
		GuiDeathMenu(GuiController* controller) : GuiDialog(controller), returnToMenu(0), taunt(0) {}
		virtual void init();
		virtual void handleEvent(const SEvent& event);
	private:
		IGUIButton* returnToMenu;
		IGUIStaticText* taunt;
};

#endif 