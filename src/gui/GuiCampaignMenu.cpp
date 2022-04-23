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
	scaleAlign(img);

	hud.orbiting = guienv->addImage(rect<s32>(position2di(150, 250), dimension2du(96, 96)), root);
	hud.orbiting->setImage(driver->getTexture("ui/planet.png"));
	scaleAlign(hud.orbiting);

	hud.shipSprite = guienv->addImage(rect<s32>(position2di(300, 280), dimension2du(46, 17)), root);
	hud.shipSprite->setImage(driver->getTexture("ui/shipsprite.png"));
	scaleAlign(hud.shipSprite);

	hud.HUDimg = guienv->addImage(rect<s32>(position2di(0, 0), baseSize), root);
	hud.HUDimg->setImage(driver->getTexture("ui/campaignbase.png"));
	scaleAlign(hud.HUDimg);

	hud.toMenu = guienv->addButton(rect<s32>(position2di(0, 0), dimension2du(90, 55)), hud.HUDimg, CAMPAIGN_TO_MENU, L"Menu", L"Back out for now.");
	setHoloButton(hud.toMenu);
	guiController->setCallback(hud.toMenu, std::bind(&GuiCampaignMenu::onMenu, this, std::placeholders::_1));


	start = guienv->addButton(rect<s32>(position2di(480, 150), dimension2du(90, 55)), root, CAMPAIGN_START_BUTTON, L"Start Scenario!", L"Fear not the enemy. Fear your teammates.");
	setMetalButton(start);
	guiController->setCallback(start, std::bind(&GuiCampaignMenu::onStart, this, std::placeholders::_1));


	loadout.img = guienv->addImage(rect<s32>(position2di(286, 470), dimension2du(388,359)), root);
	loadout.img->setImage(driver->getTexture("ui/loadout.png"));
	
	loadout.button = guienv->addButton(rect<s32>(position2di(67, 25), dimension2du(260, 40)), loadout.img, CAMPAIGN_LOADOUT, L"Loadouts", L"View loadouts.");
	setHoloButton(loadout.button);
	scaleAlign(loadout.img);
	guiController->setCallback(loadout.button, std::bind(&GuiCampaignMenu::onLoadout, this, std::placeholders::_1));
	guiController->setAnimationCallback(loadout.button, std::bind(&GuiCampaignMenu::moveLoadout, this, std::placeholders::_1));
	hide();
}

void GuiCampaignMenu::show()
{
	root->setRelativePosition(rect<s32>(position2di(0, 0), driver->getScreenSize()));
	root->setVisible(true);
}

bool GuiCampaignMenu::onStart(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	stateController->setState(GAME_RUNNING);
	return false;
}

bool GuiCampaignMenu::onMenu(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	guiController->setActiveDialog(GUI_MAIN_MENU);
	return false;
}

bool GuiCampaignMenu::onLoadout(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	loadout.timer = 0;
	loadout.isOpen = !loadout.isOpen;
	guiController->callAnimation(loadout.button);
	return false;
}

bool GuiCampaignMenu::moveLoadout(f32 dt)
{
	rect<s32> relpos = loadout.img->getRelativePosition();
	position2di center = relpos.getCenter();

	position2di pos(center.X - relpos.getWidth() / 2, center.Y - relpos.getHeight() / 2);
	rect<s32> screen = root->getAbsolutePosition();

	s32 openHeight = screen.getHeight() - relpos.getHeight();
	s32 closeHeight = screen.getHeight() - relpos.getHeight() / 5;

	vector2di open(pos.X, openHeight);
	vector2di close(pos.X, closeHeight);
	f32 animTime = .1f;

	if (loadout.isOpen) {
		return smoothGuiMove(loadout.img, animTime, loadout.timer, close, open, dt);
	}
	else {
		return smoothGuiMove(loadout.img, animTime, loadout.timer, open, close, dt);
	}
}