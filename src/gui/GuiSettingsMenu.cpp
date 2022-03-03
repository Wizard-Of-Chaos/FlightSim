#include "GuiSettingsMenu.h"
#include "GuiController.h"
#include "GameStateController.h"

void GuiSettingsMenu::init()
{
	u32 height = baseSize.Height / 8;
	u32 width = baseSize.Width / 4;
	dimension2du size(width,height);
	filtering = guiController->guienv->addComboBox(rect<s32>(position2di(16, 16), size), root, SETTINGSMENU_FILTERING_BOX);
	filtering->addItem(L"Bilinear");
	filtering->addItem(L"Trilinear");
	filtering->addItem(L"Anisotropic");
	resolution = guiController->guienv->addComboBox(rect<s32>(position2di(16, height + 16), size), root, SETTINGSMENU_RESOLUTION);
	resolution->addItem(L"Auto");
	resolution->addItem(L"1920x1080");
	resolution->addItem(L"960x540");
	fullscreen = guiController->guienv->addCheckBox(false, rect<s32>(position2di(16, (height * 2) + 16), size), root, SETTINGSMENU_FULLSCREEN, L"Fullscreen");
	vsync = guiController->guienv->addCheckBox(false, rect<s32>(position2di(16, (height * 3) + 16), size), root, SETTINGSMENU_VSYNC, L"Vsync");
	antialiasing = guiController->guienv->addCheckBox(false, rect<s32>(position2di(16, (height * 4) + 16), size), root, SETTINGSMENU_ANTIALIASING, L"Antialiasing");
	shadows = guiController->guienv->addCheckBox(false, rect<s32>(position2di(16, (height * 5) + 16), size), root, SETTINGSMENU_SHADOWS, L"Shadows");

	keybinds = guiController->guienv->addButton(rect<s32>(position2di(width + 16, 16), size), root, SETTINGSMENU_KEYBINDS, L"Set Keybinds", L"Adjust keybinds.");
	returnToMain = guiController->guienv->addButton(rect<s32>(position2di(width  + 16, height + 16), size), root, SETTINGSMENU_RETURN_TO_MAIN, L"Return To Main", L"Restart the game to apply settings.");
	restart = guiController->guienv->addStaticText(L"A restart is required to apply settings.", rect<s32>(position2di(baseSize.Width - width, 32), size), false, true, root);
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

	hide();
}

void GuiSettingsMenu::restartRequired()
{
	restartReminder = true;
	restart->setVisible(true);
}

void GuiSettingsMenu::handleEvent(const SEvent& event)
{
	switch (event.EventType)
	{
	case EET_MOUSE_INPUT_EVENT:
		break;
	case EET_KEY_INPUT_EVENT:
		break;
	case EET_GUI_EVENT:
		s32 id = event.GUIEvent.Caller->getID();
		switch (event.GUIEvent.EventType) {
		case EGET_BUTTON_CLICKED:
			switch (id) {
			case SETTINGSMENU_RETURN_TO_MAIN:
				guiController->setActiveDialog(GUI_MAIN_MENU);
				break;
			case SETTINGSMENU_KEYBINDS:
				break;
			}
			break;
		case EGET_CHECKBOX_CHANGED:
			switch (id) {
			case SETTINGSMENU_FULLSCREEN:
				restartRequired();
				break;
			case SETTINGSMENU_VSYNC:
				restartRequired();
				break;
			case SETTINGSMENU_ANTIALIASING:
				restartRequired();
				break;
			case SETTINGSMENU_SHADOWS:
				restartRequired();
				break;
			}
			break;
		case EGET_COMBO_BOX_CHANGED:
			switch (id) {
			case SETTINGSMENU_FILTERING_BOX:
				restartRequired();
				break;
			case SETTINGSMENU_RESOLUTION:
				restartRequired();
				break;
			}
			break;
		}
	}
}