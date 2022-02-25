#pragma once
#ifndef GUIDEATHMENU_H
#define GUIDEATHMENU_H
#include "BaseHeader.h"
#include "GuiDialog.h"
#include "GvReader.h"
#include <random>

//Enum holding the IDs for buttons on the death menu.
enum DEATH_MENU_BUTTONS
{
	DEATHMENU_RETURN
};

/*
* The death menu displays when the player dies. It also mocks the player. Includes the buttons, logic
* to handle events, and a bit of text as a taunt pulled from the taunt file.
*/
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