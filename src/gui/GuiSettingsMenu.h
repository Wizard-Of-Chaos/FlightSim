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
};
#endif 