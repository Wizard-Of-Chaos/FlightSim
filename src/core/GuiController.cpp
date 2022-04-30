#include "GuiController.h"
#include "GameStateController.h"
#include <iostream>

GuiController::GuiController()
{
	activeDialog = 0;

	gvReader tauntReader;
	tauntReader.read("attributes/taunts.txt");
	for (std::string line : tauntReader.lines) {
		taunts.push_back(std::wstring(line.begin(), line.end()));
	}
	tauntReader.clear();
	tauntReader.read("attributes/congrats.txt");
	for (std::string line : tauntReader.lines) {
		congrats.push_back(wstr(line));
	}
	dimension2du baseSize(960, 540);
	u32 horizpos = (baseSize.Width - 512) / 2;
	u32 vertpos = (baseSize.Height - 512) / 2;

	okPopup.bg = guienv->addImage(rect<s32>(position2di(horizpos, vertpos), dimension2du(512, 512)));
	okPopup.bg->setImage(driver->getTexture("ui/popup.png"));
	okPopup.button = guienv->addButton(rect<s32>(position2di(192, 448), dimension2du(128, 64)), okPopup.bg, 2500, L"Got it");
	okPopup.title = guienv->addStaticText(L"Heads up", rect<s32>(position2di(64, 24), dimension2du(384, 24)), false, true, okPopup.bg);
	okPopup.body = guienv->addStaticText(L"", rect<s32>(position2di(64, 16), dimension2du(384, 400)), false, true, okPopup.bg);
	scaleAlign(okPopup.bg);
	setMetalButton(okPopup.button);
	setUIText(okPopup.title);
	setUIText(okPopup.body);

	yesNoPopup.bg = guienv->addImage(rect<s32>(position2di(horizpos, vertpos), dimension2du(512, 512)));
	yesNoPopup.bg->setImage(driver->getTexture("ui/popup.png"));
	yesNoPopup.title = guienv->addStaticText(L"Heads up", rect<s32>(position2di(64, 24), dimension2du(384, 24)), false, true, yesNoPopup.bg);
	yesNoPopup.body = guienv->addStaticText(L"", rect<s32>(position2di(64, 16), dimension2du(384, 400)), false, true, yesNoPopup.bg);
	yesNoPopup.yes = guienv->addButton(rect<s32>(position2di(126, 448), dimension2du(128, 64)), yesNoPopup.bg, 2501, L"Yes");
	yesNoPopup.no = guienv->addButton(rect<s32>(position2di(256, 448), dimension2du(128, 64)), yesNoPopup.bg, 2502, L"No");
	scaleAlign(yesNoPopup.bg);
	setMetalButton(yesNoPopup.yes);
	setMetalButton(yesNoPopup.no);
	setUIText(yesNoPopup.title);
	setUIText(yesNoPopup.body);

	okPopup.bg->setVisible(false);
	yesNoPopup.bg->setVisible(false);
	setCallback(okPopup.button, std::bind(&GuiController::hidePopup, this, std::placeholders::_1));
	setCallback(yesNoPopup.no, std::bind(&GuiController::hidePopup, this, std::placeholders::_1));
}

void GuiController::init()
{
	//All menus get initialized here. Don't delete them.
	//If you've just added a new menu, initialize it here.
	menus.menuDialogs[GUI_MAIN_MENU] = new GuiMainMenu;
	menus.menuDialogs[GUI_MAIN_MENU]->init();
	menus.menuDialogs[GUI_PAUSE_MENU] = new GuiPauseMenu;
	menus.menuDialogs[GUI_PAUSE_MENU]->init();
	menus.menuDialogs[GUI_DEATH_MENU] = new GuiDeathMenu;
	menus.menuDialogs[GUI_DEATH_MENU]->init();
	menus.menuDialogs[GUI_SETTINGS_MENU] = new GuiSettingsMenu;
	menus.menuDialogs[GUI_SETTINGS_MENU]->init();
	menus.menuDialogs[GUI_LOADOUT_MENU] = new GuiLoadoutMenu;
	menus.menuDialogs[GUI_LOADOUT_MENU]->init();
	menus.menuDialogs[GUI_CAMPAIGN_MENU] = new GuiCampaignMenu;
	menus.menuDialogs[GUI_CAMPAIGN_MENU]->init();

	setActiveDialog(GUI_MAIN_MENU);
	//default main menu
}

void GuiController::setOkPopup(std::string title, std::string body, std::string button)
{
	okPopup.title->setText(wstr(title).c_str());
	okPopup.body->setText(wstr(body).c_str());
	okPopup.button->setText(wstr(button).c_str());
}

void GuiController::setYesNoPopup(std::string title, std::string body, GuiCallback yesCb, std::string yes, std::string no)
{
	yesNoPopup.title->setText(wstr(title).c_str());
	yesNoPopup.body->setText(wstr(body).c_str());
	yesNoPopup.yes->setText(wstr(yes).c_str());
	yesNoPopup.no->setText(wstr(no).c_str());

	removeCallback(yesNoPopup.yes);
	setCallback(yesNoPopup.yes, yesCb);
}

void GuiController::showOkPopup()
{
	dimension2du screenSize = driver->getScreenSize();
	u32 horizpos = (screenSize.Width - 512) / 2;
	u32 vertpos = (screenSize.Height - 512) / 2;
	okPopup.bg->setRelativePosition(position2di(horizpos, vertpos));
	okPopup.bg->setVisible(true);
	guienv->getRootGUIElement()->bringToFront(okPopup.bg);
	popupActive = true;
}

void GuiController::showYesNoPopup()
{
	dimension2du screenSize = driver->getScreenSize();
	u32 horizpos = (screenSize.Width - 512) / 2;
	u32 vertpos = (screenSize.Height - 512) / 2;
	yesNoPopup.bg->setRelativePosition(position2di(horizpos, vertpos));
	yesNoPopup.bg->setVisible(true);
	guienv->getRootGUIElement()->bringToFront(yesNoPopup.bg);
	popupActive = true;
}
bool GuiController::hidePopup(const SEvent& event)
{
	if (event.GUIEvent.EventType != EGET_BUTTON_CLICKED || 
		(event.GUIEvent.Caller != okPopup.button) && (event.GUIEvent.Caller != yesNoPopup.yes) && (event.GUIEvent.Caller != yesNoPopup.no)) return true;
	okPopup.bg->setVisible(false);
	yesNoPopup.bg->setVisible(false);
	popupActive = false;
	return false;
}

void GuiController::setAnimationCallback(IGUIElement* elem, AnimationCallback cb)
{
	animationCallbacks[elem] = cb;
}
void GuiController::setOpenAnimationCallback(GuiDialog* dialog, AnimationCallback cb)
{
	openAnimationCallbacks[dialog] = cb;
}
void GuiController::setCloseAnimationCallback(GuiDialog* dialog, AnimationCallback cb)
{
	closeAnimationCallbacks[dialog] = cb;
}

void GuiController::callCloseAnimation(GuiDialog* dialog)
{
	if (closeAnimationCallbacks.find(dialog) != closeAnimationCallbacks.end()) {
		currentAnimation = closeAnimationCallbacks[dialog];
		playingAnimation = true;
	}
}
void GuiController::callOpenAnimation(GuiDialog* dialog)
{
	if (openAnimationCallbacks.find(dialog) != openAnimationCallbacks.end()) {
		currentAnimation = openAnimationCallbacks[dialog];
		playingAnimation = true;
	}
}
void GuiController::callAnimation(IGUIElement* elem)
{
	if (animationCallbacks.find(elem) != animationCallbacks.end() && !playingAnimation) {
		currentAnimation = animationCallbacks[elem];
		playingAnimation = true;
	}
}

std::wstring GuiController::getTaunt()
{
	return taunts[std::rand() % taunts.size()]; //Pulls out a random taunt to mess with the player
}

std::wstring GuiController::getCongrats()
{
	return congrats[std::rand() % congrats.size()];
}

void GuiController::close()
{
	if (activeDialog) {
		activeDialog->hide();
		activeDialog = nullptr;
	} //Doesn't actually delete anything; that's what menuCleanup is for (and guienv->clear() is called elsewhere to actually remove elements)
}

bool GuiController::OnEvent(const SEvent& event)
{
	//Hurls the event to the active dialog.
	if (activeDialog && event.EventType == EET_GUI_EVENT && callbacks.find(event.GUIEvent.Caller) != callbacks.end()) {
		if(!popupActive) return callbacks[event.GUIEvent.Caller](event);

		if (event.GUIEvent.Caller == yesNoPopup.yes) {
			callbacks[yesNoPopup.yes](event);
		}
		return hidePopup(event);
	}
	return true;
}

void GuiController::setCallback(IGUIElement* elem, GuiCallback callback)
{
	callbacks[elem] = callback;
}

void GuiController::removeCallback(IGUIElement* elem)
{
	callbacks.erase(elem);
}

void GuiController::update()
{
	u32 now = device->getTimer()->getTime();
	f32 delta = (f32)(now - then) / 1000.f;
	if (delta > .25) { //If the delta is too big, it's .25.
		delta = .25;
	}
	then = now;
	accumulator += delta;
	while (accumulator >= dt) {
		if (playingAnimation) {
			playingAnimation = currentAnimation(dt);
		}

		if (!playingAnimation && switchMenusCalled) {
			if (activeDialog) activeDialog->hide();

			activeDialog = menus.menuDialogs[menuToSwitch];
			activeDialog->show();
			callOpenAnimation(activeDialog);
			guienv->getRootGUIElement()->bringToFront(activeDialog->getRoot());
			switchMenusCalled = false;
		}

		t += dt;
		accumulator -= dt;
	}
}

//If you've just added a new menu, go make sure that you added it as a menu type in MenuData.h
void GuiController::setActiveDialog(MENU_TYPE menu)
{
	if(activeDialog) callCloseAnimation(activeDialog);
	menuToSwitch = menu;
	switchMenusCalled = true;
}