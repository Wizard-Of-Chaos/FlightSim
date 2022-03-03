#pragma once
#ifndef GUISETTINGSMENU_H
#define GUISETTINGSMENU_H
#include "BaseHeader.h"
#include "GuiDialog.h"

enum SETTINGS_MENU_BUTTONS {
	SETTINGSMENU_FILTERING_BOX,
	SETTINGSMENU_RETURN_TO_MAIN,
	SETTINGSMENU_FULLSCREEN,
	SETTINGSMENU_KEYBINDS,
	SETTINGSMENU_RESOLUTION,
	SETTINGSMENU_VSYNC,
	SETTINGSMENU_ANTIALIASING,
	SETTINGSMENU_SHADOWS
};

class GuiSettingsMenu : public GuiDialog
{
	public:
		GuiSettingsMenu(GuiController* controller) : GuiDialog(controller), filtering(0), fullscreen(0), returnToMain(0) {}
		virtual void init();
		virtual void handleEvent(const SEvent& event);
		void restartRequired();
	private:
		IGUIComboBox* filtering;
		IGUIComboBox* resolution;
		IGUICheckBox* fullscreen;
		IGUICheckBox* vsync;
		IGUICheckBox* antialiasing;
		IGUICheckBox* shadows;
		IGUIButton* returnToMain;
		IGUIButton* keybinds;
		IGUIStaticText* restart;
		bool restartReminder;
};
#endif 