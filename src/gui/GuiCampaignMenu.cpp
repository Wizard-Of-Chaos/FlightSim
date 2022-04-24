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

	scenariohud.areadesc = guienv->addImage(rect<s32>(position2di(960, 114), dimension2du(305, 311)), root);
	scenariohud.areadesc->setImage(driver->getTexture("ui/areadesc.png"));
	scaleAlign(scenariohud.areadesc);
	scenariohud.launch = guienv->addImage(rect<s32>(position2di(362, 4), dimension2du(236, 51)), root);
	scenariohud.launch->setImage(driver->getTexture("ui/launch.png"));
	scaleAlign(scenariohud.launch);
	scenariohud.launchButton = guienv->addButton(rect<s32>(position2di(54, 6), dimension2du(135, 20)), scenariohud.launch, -1, L"Start", L"Shoot mans!");
	setHoloButton(scenariohud.launchButton);
	guiController->setCallback(scenariohud.launchButton, std::bind(&GuiCampaignMenu::onStart, this, std::placeholders::_1));

	hud.sectorHeader = guienv->addImage(rect<s32>(position2di(290, 0), dimension2du(380, 55)), root);
	hud.sectorHeader->setImage(driver->getTexture("ui/header.png"));
	scaleAlign(hud.sectorHeader);

	start = guienv->addButton(rect<s32>(position2di(480, 150), dimension2du(90, 55)), root, CAMPAIGN_START_BUTTON, L"schmoove", L"Move the GUI.");
	setMetalButton(start);
	guiController->setCallback(start, std::bind(&GuiCampaignMenu::onMoveGui, this, std::placeholders::_1));

	loadout.img = guienv->addImage(rect<s32>(position2di(286, 470), dimension2du(388,359)), root);
	loadout.img->setImage(driver->getTexture("ui/loadout.png"));
	scaleAlign(loadout.img);
	
	loadout.button = guienv->addButton(rect<s32>(position2di(67, 25), dimension2du(260, 40)), loadout.img, CAMPAIGN_LOADOUT, L"Loadouts", L"View loadouts.");
	setHoloButton(loadout.button);

	guiController->setCallback(loadout.button, std::bind(&GuiCampaignMenu::onLoadout, this, std::placeholders::_1));
	guiController->setAnimationCallback(loadout.button, std::bind(&GuiCampaignMenu::moveLoadout, this, std::placeholders::_1));
	guiController->setAnimationCallback(start, std::bind(&GuiCampaignMenu::moveSectorInfo, this, std::placeholders::_1));
	hide();
}

void GuiCampaignMenu::show()
{
	root->setRelativePosition(rect<s32>(position2di(0, 0), driver->getScreenSize()));
	root->setVisible(true);
}

bool GuiCampaignMenu::onMoveGui(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	guiController->callAnimation(start);
	return false;
}

bool GuiCampaignMenu::onStart(const SEvent& event)
{

	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	guiController->callAnimation(start);
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

bool GuiCampaignMenu::moveSectorInfo(f32 dt)
{
	f32 launchRatioOpen = 4.f / 540.f;
	f32 launchRatioClosed = 55.f / 540.f;
	f32 areaRatioOpen = 305.f / 960.f;

	rect<s32> launch = scenariohud.launch->getRelativePosition();
	rect<s32> area = scenariohud.areadesc->getRelativePosition();
	rect<s32> screen = root->getAbsolutePosition();
	position2di launchPos(launch.UpperLeftCorner);
	position2di areaPos(area.UpperLeftCorner);

	vector2di launchOpen(launchPos.X, (s32)(screen.getHeight() * launchRatioOpen));
	vector2di launchClosed(launchPos.X, (s32)(screen.getHeight() * launchRatioClosed));
	vector2di areaOpen(screen.getWidth() - (s32)(screen.getWidth() * areaRatioOpen), areaPos.Y);
	vector2di areaClosed(screen.getWidth(), areaPos.Y);

	bool move;
	if (sectorInfoShowing) {
		smoothGuiMove(scenariohud.areadesc, .1f, scenariohud.timer1, areaOpen, areaClosed, dt);
		move = smoothGuiMove(scenariohud.launch, .1f, scenariohud.timer2, launchClosed, launchOpen, dt);
	}
	else {
		smoothGuiMove(scenariohud.areadesc, .1f, scenariohud.timer1, areaClosed, areaOpen, dt);
		move = smoothGuiMove(scenariohud.launch, .1f, scenariohud.timer2, launchOpen, launchClosed, dt);
	}
	if (!move) {
		sectorInfoShowing = !sectorInfoShowing;
	}
	return move;
	//return false;
}