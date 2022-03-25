#pragma once
#ifndef GUISETTINGSMENU_H
#define GUISETTINGSMENU_H
#include "BaseHeader.h"
#include "GuiDialog.h"
#include "Config.h"

//Buttons for the settings menu.
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

/*
* The GuiSettingsMenu should be pretty self explanatory; what it does is keep track of the buttons for enabling / disabling
* video options as well as gameplay options and keybinds.
*/
class GuiSettingsMenu : public GuiDialog
{
	public:
		GuiSettingsMenu(GuiController* controller) : GuiDialog(controller) {}
		virtual void init();

		void restartRequired();

		bool onFilterSelect(const SEvent& event);
		bool onResolutionSelect(const SEvent& event);
		bool onCheckFullscreen(const SEvent& event);
		bool onCheckVsync(const SEvent& event);
		bool onCheckAliasing(const SEvent& event);
		bool onCheckShadows(const SEvent& event);
		bool onReturn(const SEvent& event);
		bool onKeybinds(const SEvent& event);

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
		VideoConfig* vConfig;
};
#endif 