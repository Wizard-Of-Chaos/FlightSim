#include "GuiDeathMenu.h"
#include "GuiController.h"
#include "GameStateController.h"
#include <time.h>

void GuiDeathMenu::init()
{
	u32 verticalSlice = baseSize.Height / 6;
	u32 horizontalPos = baseSize.Width / 3;
	dimension2du buttonSize(horizontalPos, verticalSlice);

	returnToMenu = guiController->guienv->addButton(rect<s32>(position2di(horizontalPos, verticalSlice + 32), buttonSize), root, DEATHMENU_RETURN, L"Return To Menu", L"Loooser!");
	taunt = guiController->guienv->addStaticText(guiController->getTaunt().c_str(), rect<s32>(position2di(horizontalPos, 32), buttonSize), false, true, root);
	taunt->setOverrideColor(SColor(255, 210, 210, 210));

	scaleAlign(returnToMenu);
	scaleAlign(taunt);

	returnToMenu->setScaleImage(true);
	hide();
}

void GuiDeathMenu::handleEvent(const SEvent& event)
{
	switch (event.EventType)
	{
	case EET_MOUSE_INPUT_EVENT:
		break;
	case EET_KEY_INPUT_EVENT:
		break;
	case EET_GUI_EVENT:
		s32 id = event.GUIEvent.Caller->getID();
		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			switch (id) 
			{
			case DEATHMENU_RETURN:
				taunt->setText(guiController->getTaunt().c_str());
				guiController->stateController->setState(GAME_MENUS);
				break;
			default:
				break;
			}
		}
	}
}