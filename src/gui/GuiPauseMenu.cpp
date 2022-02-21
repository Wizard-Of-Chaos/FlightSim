#include "GuiPauseMenu.h"
#include "GuiController.h"
#include "GameStateController.h"

void GuiPauseMenu::init()
{
	dimension2du screenSize = guiController->driver->getScreenSize();
	u32 verticalSlice = baseSize.Height / 6;
	u32 horizontalPos = baseSize.Width / 3;
	dimension2du buttonSize(horizontalPos, verticalSlice);

	resumeGame = guiController->guienv->addButton(rect<s32>(position2di(horizontalPos, 32), buttonSize), root, PAUSEMENU_RESUME, L"Resume Game", L"Get back in there!");
	pauseSettings = guiController->guienv->addButton(rect<s32>(position2di(horizontalPos, 32 * 2 + verticalSlice), buttonSize), root, PAUSEMENU_SETTINGS, L"Settings", L"What, is your sensitivity too low?");
	exitToMenus = guiController->guienv->addButton(rect<s32>(position2di(horizontalPos, 32 * 3 + verticalSlice * 2), buttonSize), root, PAUSEMENU_EXIT, L"Exit to Main Menu", L"Run, coward!");
	scaleAlign(resumeGame);
	scaleAlign(pauseSettings);
	scaleAlign(exitToMenus);
	resumeGame->setScaleImage(true);
	pauseSettings->setScaleImage(true);
	exitToMenus->setScaleImage(true);

	hide();
}

void GuiPauseMenu::handleEvent(const SEvent& event)
{
	switch (event.EventType) {
	case EET_MOUSE_INPUT_EVENT:
		break;
	case EET_KEY_INPUT_EVENT:
		break;
	case EET_GUI_EVENT:
		s32 id = event.GUIEvent.Caller->getID();
		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			switch (id) {
			case PAUSEMENU_RESUME:
				guiController->stateController->setState(GAME_RUNNING);
				break;
			case PAUSEMENU_SETTINGS:
				break;
			case PAUSEMENU_EXIT:
				guiController->stateController->setState(GAME_MENUS);
				break;
			}
		}
		break;
	}
}