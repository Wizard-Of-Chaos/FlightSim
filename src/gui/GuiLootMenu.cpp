#include "GuiLootMenu.h"
#include "GameStateController.h"
#include "GameController.h"
#include "GuiController.h"

void GuiLootMenu::init()
{
	screen = guienv->addImage(rect<s32>(position2di((960 - 400) / 2, -500), dimension2du(400, 500)), root);
	screen->setImage(driver->getTexture("ui/vertscreen.png"));
	scaleAlign(screen);

}
void GuiLootMenu::show()
{

}

bool GuiLootMenu::onReturnToCampaign(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	
	return false;
}

bool GuiLootMenu::onShow(f32 dt)
{
	return false;
}

bool GuiLootMenu::onHide(f32 dt)
{
	return false;
}