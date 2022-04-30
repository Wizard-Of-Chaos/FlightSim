#include "GuiCampaignLoadoutMenu.h"
#include "GameController.h"
#include "GuiController.h"
#include "GameStateController.h"

void GuiCampaignLoadoutMenu::init()
{
	backToCampaign = guienv->addButton(rect<s32>(position2di(1, 1), dimension2du(100, 30)), root, -1, L"Back", L"Return to campaign menu.");
	setMetalButton(backToCampaign);
	ships = guienv->addListBox(rect<s32>(position2di(1, 45), dimension2du(100, 300)), root, -1);
	scaleAlign(ships);

	guiController->setCallback(backToCampaign, std::bind(&GuiCampaignLoadoutMenu::onBack, this, std::placeholders::_1));
	hide();
}

void GuiCampaignLoadoutMenu::show()
{
	root->setRelativePosition(rect<s32>(position2di(0, 0), driver->getScreenSize()));
	root->setVisible(true);
	ships->clear();
	for (u32 i = 0; i < stateController->campaign.availableShips.size(); ++i) {
		u32 shipid = stateController->campaign.availableShips[i].ship.shipDataId;
		ships->addItem(wstr(stateController->shipData[shipid]->name).c_str());
	}
}

bool GuiCampaignLoadoutMenu::onBack(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	guiController->setActiveDialog(GUI_CAMPAIGN_MENU);
	return false;
}