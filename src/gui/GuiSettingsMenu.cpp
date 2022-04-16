#include "GuiSettingsMenu.h"
#include "GuiController.h"
#include "GameStateController.h"
#include <iostream>

void GuiSettingsMenu::init()
{
	vConfig = &stateController->videoConfig;

	u32 height = baseSize.Height / 8;
	u32 width = baseSize.Width / 4;
	dimension2du size(width,height);
	filtering = guienv->addComboBox(rect<s32>(position2di(16, 16), size), root, SETTINGSMENU_FILTERING_BOX);
	filtering->addItem(L"Bilinear");
	filtering->addItem(L"Trilinear");
	filtering->addItem(L"Anisotropic");
	resolution = guienv->addComboBox(rect<s32>(position2di(16, height + 16), size), root, SETTINGSMENU_RESOLUTION);
	resolution->addItem(L"Auto");
	resolution->addItem(L"1920x1080");
	resolution->addItem(L"960x540");
	fullscreen = guienv->addCheckBox(false, rect<s32>(position2di(16, (height * 2) + 16), size), root, SETTINGSMENU_FULLSCREEN, L"Fullscreen");
	vsync = guienv->addCheckBox(false, rect<s32>(position2di(16, (height * 3) + 16), size), root, SETTINGSMENU_VSYNC, L"Vsync");
	antialiasing = guienv->addCheckBox(false, rect<s32>(position2di(16, (height * 4) + 16), size), root, SETTINGSMENU_ANTIALIASING, L"Antialiasing");
	shadows = guienv->addCheckBox(false, rect<s32>(position2di(16, (height * 5) + 16), size), root, SETTINGSMENU_SHADOWS, L"Shadows");

	keybinds = guienv->addButton(rect<s32>(position2di(width + 16, 16), size), root, SETTINGSMENU_KEYBINDS, L"Set Keybinds", L"Adjust keybinds.");
	returnToMain = guienv->addButton(rect<s32>(position2di(width  + 16, height + 16), size), root, SETTINGSMENU_RETURN_TO_MAIN, L"Return To Main", L"Restart the game to apply settings.");
	restart = guienv->addStaticText(L"A restart is required to apply settings.", rect<s32>(position2di(baseSize.Width - width, 32), size), false, true, root);
	restartReminder = false;
	restart->setVisible(false);
	restart->setOverrideColor(SColor(255, 255, 255, 255));
	scaleAlign(filtering);
	scaleAlign(fullscreen);
	scaleAlign(returnToMain);
	scaleAlign(resolution);
	scaleAlign(vsync);
	scaleAlign(antialiasing);
	scaleAlign(keybinds);
	scaleAlign(restart);
	scaleAlign(shadows);

	guiController->setCallback(filtering, std::bind(&GuiSettingsMenu::onFilterSelect, this, std::placeholders::_1));
	guiController->setCallback(fullscreen, std::bind(&GuiSettingsMenu::onCheckFullscreen, this, std::placeholders::_1));
	guiController->setCallback(returnToMain, std::bind(&GuiSettingsMenu::onReturn, this, std::placeholders::_1));
	guiController->setCallback(resolution, std::bind(&GuiSettingsMenu::onResolutionSelect, this, std::placeholders::_1));
	guiController->setCallback(vsync, std::bind(&GuiSettingsMenu::onCheckVsync, this, std::placeholders::_1));
	guiController->setCallback(antialiasing, std::bind(&GuiSettingsMenu::onCheckAliasing, this, std::placeholders::_1));
	guiController->setCallback(keybinds, std::bind(&GuiSettingsMenu::onKeybinds, this, std::placeholders::_1));
	guiController->setCallback(shadows, std::bind(&GuiSettingsMenu::onCheckShadows, this, std::placeholders::_1));

	//std::cout << vConfig->fullscreen << std::endl;

	fullscreen->setChecked(vConfig->fullscreen);
	vsync->setChecked(vConfig->vsync);
	antialiasing->setChecked(vConfig->antialiasing);
	shadows->setChecked(vConfig->shadows);

	hide();
}

void GuiSettingsMenu::restartRequired()
{
	restartReminder = true;
	restart->setVisible(true);
}

bool GuiSettingsMenu::onFilterSelect(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_COMBO_BOX_CHANGED) return true;
	restartRequired();
	return false;
}
bool GuiSettingsMenu::onResolutionSelect(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_COMBO_BOX_CHANGED) return true;
	restartRequired();
	return false;
}
bool GuiSettingsMenu::onCheckFullscreen(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_CHECKBOX_CHANGED) return true;
	vConfig->fullscreen = fullscreen->isChecked();
	vConfig->useScreenRes = fullscreen->isChecked();
	restartRequired();
	return false;
}
bool GuiSettingsMenu::onCheckVsync(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_CHECKBOX_CHANGED) return true;
	vConfig->vsync = vsync->isChecked();
	restartRequired();
	return false;
}
bool GuiSettingsMenu::onCheckAliasing(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_CHECKBOX_CHANGED) return true;
	vConfig->antialiasing = antialiasing->isChecked();
	restartRequired();
	return false;
}
bool GuiSettingsMenu::onCheckShadows(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_CHECKBOX_CHANGED) return true;
	vConfig->shadows = shadows->isChecked();
	restartRequired();
	return false;
}
bool GuiSettingsMenu::onReturn(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	vConfig->saveConfig("videoconfig.gdat");
	guiController->setActiveDialog(GUI_MAIN_MENU);
	return false;
}
bool GuiSettingsMenu::onKeybinds(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	return false;
}