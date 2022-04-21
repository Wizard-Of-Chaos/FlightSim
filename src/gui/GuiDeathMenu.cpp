#include "GuiDeathMenu.h"
#include "GuiController.h"
#include "GameStateController.h"
#include <time.h>

void GuiDeathMenu::init()
{
	if (root) root->remove();
	root = guienv->addStaticText(L"", rect<s32>(position2di(0, 0), baseSize));

	u32 verticalSlice = baseSize.Height / 6;
	u32 horizontalPos = baseSize.Width / 3;
	dimension2du buttonSize(horizontalPos, verticalSlice);

	returnToMenu = guienv->addButton(rect<s32>(position2di(horizontalPos, verticalSlice + 32), buttonSize), root, DEATHMENU_RETURN, L"Return To Menu", L"Loooser!");
	taunt = guienv->addStaticText(guiController->getTaunt().c_str(), rect<s32>(position2di(horizontalPos, 32), buttonSize), false, true, root);

	setUIText(taunt);
	setButtonImg(returnToMenu);

	guiController->setCallback(returnToMenu, std::bind(&GuiDeathMenu::onReturn, this, std::placeholders::_1));
	hide();
}

bool GuiDeathMenu::onReturn(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;

	taunt->setText(guiController->getTaunt().c_str());
	stateController->setState(GAME_MENUS);
	return false;
}