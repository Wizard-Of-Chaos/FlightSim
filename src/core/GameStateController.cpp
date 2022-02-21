#include "GameStateController.h"
#include <iostream>

GameStateController::GameStateController(IrrlichtDevice* dev)
{
	device = dev;
	driver = 0;
	smgr = 0;
	guienv = 0;
	soundEngine = 0;
	then = 0;
	gameController = 0;
	init();
}

void GameStateController::init()
{
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();
	device->setEventReceiver(this);
	guienv->setUserEventReceiver(this);
	then = device->getTimer()->getTime();
	state = GAME_MENUS;

	soundEngine = createIrrKlangDevice();
	soundEngine->play2D("audio/music/space_boogaloo.mp3", true);
	gameController = new GameController(this);

	guiController = new GuiController(this);
	guiController->init();

	IGUIFont* defaultFont = guienv->getFont("fonts/Courier16px/Courier16px.xml");
	if (defaultFont) {
		guienv->getSkin()->setFont(defaultFont);
	}
	IGUIFont* tooltipDefaultFont = guienv->getFont("fonts/Courier8px/Courier8px.xml");
	if (tooltipDefaultFont) {
		guienv->getSkin()->setFont(tooltipDefaultFont, EGDF_TOOLTIP);
	}
}

bool GameStateController::OnEvent(const SEvent& event)
{
	if (event.EventType == EET_KEY_INPUT_EVENT) {
		if (event.KeyInput.Key == KEY_ESCAPE && event.KeyInput.PressedDown) {
			if (state == GAME_RUNNING) {
				setState(GAME_PAUSED);
			}
			else if (state == GAME_PAUSED) {
				setState(GAME_RUNNING);
			}
		}
	}
	switch (state) {
	case GAME_MENUS:
		guiController->OnEvent(event);
		break;
	case GAME_RUNNING:
		gameController->OnEvent(event);
		break;
	case GAME_PAUSED:
		guiController->OnEvent(event);
		break;
	case GAME_DEAD:
		guiController->OnEvent(event);
		break;
	}
	return false;
}

void GameStateController::setState(GAME_STATE newState)
{
	oldState = state;
	state = newState;
	stateChangeCalled = true;
}

void GameStateController::stateChange()
{
	if (oldState == GAME_MENUS && state == GAME_RUNNING) {
		guiController->close();
		gameController->init();
		gameController->initDefaultScene();
	}
	else if (oldState == GAME_PAUSED && state == GAME_MENUS) {
		gameController->close();
		guiController->init();
		guiController->setActiveDialog(GUI_MAIN_MENU);
	}
	else if (oldState == GAME_RUNNING && state == GAME_PAUSED) {
		guiController->setActiveDialog(GUI_PAUSE_MENU);
	}
	else if (oldState == GAME_PAUSED && state == GAME_RUNNING) {
		guiController->close();
	}
	else if (oldState == GAME_RUNNING && state == GAME_DEAD) {
		guiController->setActiveDialog(GUI_DEATH_MENU);
		//set up death menu
	}
	else if (oldState == GAME_DEAD && state == GAME_MENUS) {
		gameController->close();
		guiController->init();
		guiController->setActiveDialog(GUI_MAIN_MENU);
	}

	stateChangeCalled = false;
}

void GameStateController::mainLoop()
{
	u32 lastFPS = -1;
	while (device->run()) {
		if (stateChangeCalled) {
			stateChange();
		}

		u32 now = device->getTimer()->getTime();
		switch (state) {
			case GAME_MENUS:
				guiController->update();
				break;
			case GAME_RUNNING:
				gameController->update();
				break;
			case GAME_PAUSED:
				guiController->update();
				break;
			case GAME_DEAD:
				guiController->update();
				break;
		}
		then = now;

		driver->beginScene(true, true, SColor(255, 20, 20, 20));
		smgr->drawAll();
		guienv->drawAll();
		driver->endScene();

		int fps = driver->getFPS();
		stringw tmp(L"Flight [");
		tmp += driver->getName();
		tmp += L"] FPS: ";
		if (lastFPS != fps) {
			tmp += fps;
		}
		else tmp += lastFPS;

		device->setWindowCaption(tmp.c_str());
		lastFPS = fps;
	}
}