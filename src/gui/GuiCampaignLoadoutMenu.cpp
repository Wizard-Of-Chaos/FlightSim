#include "GuiCampaignLoadoutMenu.h"
#include "GameStateController.h"
#include "GameController.h"
#include "GuiController.h"

void GuiCampaignLoadoutMenu::init()
{
	back = guienv->addButton(rect<s32>(position2di(0, 0), dimension2du(100, 40)), root, -1, L"Back", L"Return to campaign menu.");
	wepMenu = guienv->addImage(rect<s32>(position2di(10, 120), dimension2du(237, 319)), root);
	physWepMenu = guienv->addImage(rect<s32>(position2di(713, 120), dimension2du(237, 319)), root);
	wepMenu->setImage(driver->getTexture("ui/freestandingmenu.png"));
	physWepMenu->setImage(driver->getTexture("ui/freestandingmenu.png"));
	scaleAlign(wepMenu);
	scaleAlign(physWepMenu);
	setHybridButton(back, true);

	guiController->setCallback(back, std::bind(&GuiCampaignLoadoutMenu::onBack, this, std::placeholders::_1));
	hide();
}

void GuiCampaignLoadoutMenu::show()
{
	root->setRelativePosition(rect<s32>(position2di(0, 0), driver->getScreenSize()));
	root->setVisible(true);
}

bool GuiCampaignLoadoutMenu::onBack(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	guiController->setActiveDialog(GUI_CAMPAIGN_MENU);
	return false;
}