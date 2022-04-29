#include "GuiMainMenu.h"
#include "GuiController.h"
#include "GameStateController.h"
#include <iostream>

void GuiMainMenu::init()
{
	//Convenience declarations here to determine how big a button should be.
	//In this case, it's setting up some fairly large buttons straight down the middle of the screen.
	u32 verticalSlice = baseSize.Height / 6;
	u32 horizontalPos = baseSize.Width / 3;
	dimension2du buttonSize(horizontalPos, verticalSlice); //third of the screen size and a sixth of the height

	screen = guienv->addImage(rect<s32>(position2di((960 - 400) / 2, -500), dimension2du(400, 500)), root);
	screen->setImage(driver->getTexture("ui/vertscreen.png"));
	screen->setScaleImage(true);

	buttonSize = dimension2du(300, 80);
	//All buttons have the root node set as the parent. This allows a single call to root->setVisible in order to display or hide the menu.
	startGame = guienv->addButton(rect<s32>(position2di(50, 16), buttonSize), screen, MAINMENU_START, L"Start Game", L"Are you prepared to shoot rocks?");
	loadout = guienv->addButton(rect<s32>(position2di(50, 16*2 + 80), buttonSize), screen, MAINMENU_LOADOUT, L"Set Loadout", L"What guns do you like?");
	settings = guienv->addButton(rect<s32>(position2di(50, 16 * 3 + 80 * 2), buttonSize), screen, MAINMENU_SETTINGS, L"Settings", L"Like we have any worthwhile settings.");
	quitGame = guienv->addButton(rect<s32>(position2di(50, 16 * 4 + (80 * 3)), buttonSize), screen, MAINMENU_QUIT, L"Quit Game", L"You'll be back.");

	setHoloButton(startGame);
	setHoloButton(loadout);
	setHoloButton(settings);
	setHoloButton(quitGame);

	guiController->setCallback(startGame, std::bind(&GuiMainMenu::onStart, this, std::placeholders::_1));
	guiController->setCallback(loadout, std::bind(&GuiMainMenu::onLoadout, this, std::placeholders::_1));
	guiController->setCallback(settings, std::bind(&GuiMainMenu::onSettings, this, std::placeholders::_1));
	guiController->setCallback(quitGame, std::bind(&GuiMainMenu::onQuit, this, std::placeholders::_1));

	guiController->setCloseAnimationCallback(this, std::bind(&GuiMainMenu::onHide, this, std::placeholders::_1));
	guiController->setOpenAnimationCallback(this, std::bind(&GuiMainMenu::onShow, this, std::placeholders::_1));
	hide();
}

void GuiMainMenu::show()
{
	f32 horprop = 400.f / 960.f;
	f32 vertprop = 500.f / 540.f;
	root->setRelativePosition(rect<s32>(position2di(0, 0), driver->getScreenSize()));
	rect<s32> pos = root->getAbsolutePosition();

	dimension2du size((u32)(pos.getWidth() * horprop), (u32)(pos.getHeight() * vertprop));
	rect<s32> screenpos(position2di((pos.getWidth() - size.Width) / 2, -(pos.getHeight() * vertprop)), size);
	screen->setRelativePosition(screenpos);
	root->setVisible(true);
}

bool GuiMainMenu::onShow(f32 dt)
{
	rect<s32> pos = root->getAbsolutePosition();
	rect<s32> imgpos = screen->getRelativePosition();
	position2di curpos = imgpos.getCenter();
	curpos.X -= imgpos.getWidth() / 2;
	curpos.Y -= imgpos.getHeight() / 2;
	f32 vertprop = 500.f / 540.f;
	s32 dist = (s32)(pos.getHeight() * vertprop);
	return smoothGuiMove(screen, .25f, showAnimTimer, position2di(curpos.X, -dist), position2di(curpos.X, -5), dt);
}
bool GuiMainMenu::onHide(f32 dt)
{
	rect<s32> pos = root->getAbsolutePosition();
	rect<s32> imgpos = screen->getRelativePosition();
	position2di curpos = imgpos.getCenter();
	curpos.X -= imgpos.getWidth() / 2;
	curpos.Y -= imgpos.getHeight() / 2;
	f32 vertprop = 500.f / 540.f;
	s32 dist = (s32)(pos.getHeight() * vertprop);
	return smoothGuiMove(screen, .25f, showAnimTimer, position2di(curpos.X, -5), position2di(curpos.X, -dist), dt);
}

bool GuiMainMenu::onStart(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	if (!guiController->initPopupUsed) {
		guiController->setYesNoPopup("Heads up",
			"This game isn't done yet. Please be patient; the devs are kind of stupid. \n \n You might encounter bugs, glitches, horribly broken weapons or ships, and bad environmental design. We're working on it. \n \n Your feedback is of course very welcome. Ready to proceed?",
			std::bind(&GuiMainMenu::onStart, this, std::placeholders::_1));
		guiController->initPopupUsed = true;
		guiController->showYesNoPopup();
	}
	else {
		stateController->campaign = Campaign();
		for (u32 i = 0; i < NUM_SCENARIO_OPTIONS; ++i) {
			stateController->campaign.possibleScenarios[i] = randomScenario();
			//std::cout << "location: " << i << " " << stateController->campaign.possibleScenarios[i].location << std::endl;
		}
		guiController->setActiveDialog(GUI_CAMPAIGN_MENU);
	}
	//stateController->setState(GAME_RUNNING);
	return false;

}

bool GuiMainMenu::onLoadout(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;
	guiController->setActiveDialog(GUI_LOADOUT_MENU);
	return false;
}

bool GuiMainMenu::onSettings(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true; 

	guiController->setActiveDialog(GUI_SETTINGS_MENU);
	return false;
}
bool GuiMainMenu::onQuit(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED) return true;

	device->closeDevice();
	return false;
}