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

	hud.HUDimg = guienv->addImage(rect<s32>(position2di(0, 0), baseSize), root);
	hud.HUDimg->setImage(driver->getTexture("ui/campaignbase.png"));
	hud.HUDimg->setScaleImage(true);
	scaleAlign(img);
	hud.toMenu = guienv->addButton(rect<s32>(position2di(0, 0), dimension2du(100, 60)), hud.HUDimg, CAMPAIGN_TO_MENU, L"Menu", L"Back out for now.");
	setButtonImg(hud.toMenu);
	guiController->setCallback(hud.toMenu, std::bind(&GuiCampaignMenu::onMenu, this, std::placeholders::_1));

	start = guienv->addButton(rect<s32>(position2di(horizontalPos * 2, 256), buttonSize), root, CAMPAIGN_START_BUTTON, L"Start Scenario!", L"Fear not the enemy. Fear your teammates.");
	setButtonImg(start);
	guiController->setCallback(start, std::bind(&GuiCampaignMenu::onStart, this, std::placeholders::_1));

	loadout.img = guienv->addImage(rect<s32>(position2di(286, 620), dimension2du(388,359)), root);
	loadout.img->setImage(driver->getTexture("ui/loadout.png"));
	loadout.img->setScaleImage(true);
	loadout.button = guienv->addButton(rect<s32>(position2di(67, 25), dimension2du(260, 40)), loadout.img, CAMPAIGN_LOADOUT, L"Loadouts", L"View loadouts.");
	setButtonImg(loadout.button);
	scaleAlign(img);
	guiController->setCallback(loadout.button, std::bind(&GuiCampaignMenu::onLoadout, this, std::placeholders::_1));
	guiController->setAnimationCallback(loadout.button, std::bind(&GuiCampaignMenu::moveLoadout, this, std::placeholders::_1));
	hide();
}

void GuiCampaignMenu::show()
{
	root->setRelativePosition(rect<s32>(position2di(0, 0), driver->getScreenSize()));
	root->setVisible(true);
	rect<s32> pos = root->getAbsolutePosition();
	f32 horprop = 388.f / 960.f;
	f32 vertprop = 359.f / 540.f;
	dimension2du size((u32)(pos.getWidth() * horprop), (u32)(pos.getHeight() * vertprop));
	rect<s32> loadoutpos(position2di((pos.getWidth() - size.Width) / 2, (pos.getHeight() - (u32)(size.Height*.2f))), size);
	hud.HUDimg->setRelativePosition(pos);
	loadout.img->setRelativePosition(loadoutpos);
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
	s32 diff = openHeight - closeHeight;
	loadout.timer += dt;

	f32 animTime = .1f;
	if (loadout.timer > animTime) loadout.timer = animTime;

	s32 move = (dt / animTime) * diff;
	if (loadout.isOpen) { //currently opening
		loadout.img->move(vector2di(0, move));
	}
	else {
		loadout.img->move(vector2di(0, -move));
	}
	if (loadout.timer == animTime) return false;
	else return true;
}