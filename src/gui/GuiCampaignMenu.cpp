#include "GuiCampaignMenu.h"
#include "GameController.h"
#include "GameStateController.h"
#include "GuiController.h"

void GuiCampaignMenu::init()
{
	if (root) root->remove();

	root = guienv->addImage(rect<s32>(position2di(0, 0), baseSize));
	IGUIImage* img = (IGUIImage*)root;
	img->setImage(driver->getTexture("ui/starbg.png"));
	img->setScaleImage(true);
	scaleAlign(img);

	u32 verticalSlice = baseSize.Height / 8;
	u32 horizontalPos = baseSize.Width / 5;
	dimension2du buttonSize(horizontalPos, verticalSlice);
	start = guienv->addButton(rect<s32>(position2di(horizontalPos*2, 16), buttonSize), root, CAMPAIGN_START_BUTTON, L"Start Scenario!", L"Fear not the enemy. Fear your teammates.");
	setButtonImg(start);
	guiController->setCallback(start, std::bind(&GuiCampaignMenu::onStart, this, std::placeholders::_1));
	hide();
}

bool GuiCampaignMenu::onStart(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	stateController->setState(GAME_RUNNING);
	return false;
}