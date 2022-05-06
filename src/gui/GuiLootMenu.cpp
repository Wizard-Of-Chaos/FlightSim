#include "GuiLootMenu.h"
#include "GameStateController.h"
#include "GameController.h"
#include "GuiController.h"

void GuiLootMenu::init()
{
	screen = guienv->addImage(rect<s32>(position2di((960 - 400) / 2, -500), dimension2du(400, 500)), root);
	screen->setImage(driver->getTexture("ui/vertscreen.png"));
	scaleAlign(screen);

	dimension2du buttonSize(300, 80);
	loot = guienv->addStaticText(L"Loot", rect<s32>(position2di(50, 5), dimension2du(300, 350)), false, true, screen);
	setUIText(loot);
	returnToCampaign = guienv->addButton(rect<s32>(position2di(50, 354), buttonSize), screen, -1, L"Back to Campaign", L"Return to the campaign.");
	setHoloButton(returnToCampaign);
	guiController->setCallback(returnToCampaign, std::bind(&GuiLootMenu::onReturnToCampaign, this, std::placeholders::_1));

	guiController->setCloseAnimationCallback(this, std::bind(&GuiLootMenu::onHide, this, std::placeholders::_1));
	guiController->setOpenAnimationCallback(this, std::bind(&GuiLootMenu::onShow, this, std::placeholders::_1));
	hide();
}

void GuiLootMenu::show()
{
	root->setRelativePosition(rect<s32>(position2di(0, 0), driver->getScreenSize()));
	root->setVisible(true);
}

bool GuiLootMenu::onReturnToCampaign(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	guiController->setActiveDialog(GUI_CAMPAIGN_MENU);
	return false;
}

bool GuiLootMenu::onShow(f32 dt)
{
	rect<s32> pos = root->getAbsolutePosition();
	rect<s32> imgpos = screen->getRelativePosition();
	position2di curpos = imgpos.UpperLeftCorner;
	f32 vertprop = 500.f / 540.f;
	s32 dist = (s32)(pos.getHeight() * vertprop);
	return smoothGuiMove(screen, .25f, timer, position2di(curpos.X, -dist), position2di(curpos.X, -5), dt);
}

bool GuiLootMenu::onHide(f32 dt)
{
	rect<s32> pos = root->getAbsolutePosition();
	rect<s32> imgpos = screen->getRelativePosition();
	position2di curpos = imgpos.UpperLeftCorner;
	f32 vertprop = 500.f / 540.f;
	s32 dist = (s32)(pos.getHeight() * vertprop);
	return smoothGuiMove(screen, .25f, timer, position2di(curpos.X, -5), position2di(curpos.X, -dist), dt);
}