#include "GameStateController.h"
#include <iostream>

GameStateController::GameStateController(IrrlichtDevice* dev, VideoConfig vconf)
{
	gameInitialized = false;
	device = dev;
	driver = 0;
	smgr = 0;
	guienv = 0;
	soundEngine = 0;
	then = 0;
	gameController = 0;
	videoConfig = vconf;
	init();
}

void GameStateController::init()
{
	std::cout << "Starting game... \n";

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();
	device->setEventReceiver(this);
	guienv->setUserEventReceiver(this);
	then = device->getTimer()->getTime();
	state = GAME_MENUS; //Initial state

	playerShip = 0;
	playerWeapons[0] = 1;
	playerWeapons[1] = 1;
	for (int i = 2; i < MAX_HARDPOINTS; ++i) {
		playerWeapons[i] = WEAPONID_INVALID;
	}
	loadShipAndWeaponData();

	soundEngine = createIrrKlangDevice();
	soundEngine->play2D("audio/music/space_boogaloo.ogg", true); //Todo: call this somewhere else
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
	std::cout << "Game initialized!\n";
	gameInitialized = true;
}

void GameStateController::loadShipAndWeaponData()
{
	std::string basepath = "attributes/";
	std::string weaponpath = basepath + "weapons/";
	std::string shippath = basepath + "ships/";

	gvReader in;
	std::cout << "Loading ships... \n";
	for (const auto& file : std::filesystem::directory_iterator(shippath)) {
		loadShipData(file.path().string(), this, in);
		in.clear();
	}
	std::cout << "Loading weapons... \n";
	for (const auto& file : std::filesystem::directory_iterator(weaponpath)) {
		loadWeaponData(file.path().string(), this, in);
		in.clear();
	}
	std::cout << "Done. \n";
	std::cout << "Number of weapons: " << weaponData.size() << std::endl;
	std::cout << "Number of ships: " << shipData.size() << std::endl;
}

bool GameStateController::OnEvent(const SEvent& event)
{
	if (!gameInitialized) return true;

	if (event.EventType == EET_KEY_INPUT_EVENT) {
		if (event.KeyInput.Key == KEY_ESCAPE && event.KeyInput.PressedDown) {
			if (state == GAME_RUNNING) {
				setState(GAME_PAUSED); //Overrides anything else and sets the game to paused when the escape key is hit - needs abstraction later
			}
			else if (state == GAME_PAUSED) {
				setState(GAME_RUNNING);
			}
		}
	}
	switch (state) { //Passes events to their respective controllers and tells them its THEIR problem now
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
	stateChangeCalled = true; //Lets the stateController know it needs to update the state. Can be called anywhere
}

void GameStateController::stateChange() //Messy handler for the different states; since there aren't many it's just an if chain
{
	if (oldState == GAME_MENUS && state == GAME_RUNNING) {
		guiController->close();
		gameController->init();
		gameController->initDefaultScene();
	}
	else if (oldState == GAME_PAUSED && state == GAME_MENUS) {
		gameController->close();
		guiController->setActiveDialog(GUI_MAIN_MENU);
	}
	else if (oldState == GAME_RUNNING && state == GAME_PAUSED) {
		guiController->setActiveDialog(GUI_PAUSE_MENU);
	}
	else if (oldState == GAME_PAUSED && state == GAME_RUNNING) {
		guiController->close();
	}
	else if (oldState == GAME_RUNNING && state == GAME_DEAD) { //BUG: HUD doesn't get deleted
		guiController->setActiveDialog(GUI_DEATH_MENU);
		//set up death menu
	}
	else if (oldState == GAME_DEAD && state == GAME_MENUS) {
		gameController->close();
		guiController->setActiveDialog(GUI_MAIN_MENU);
	}

	stateChangeCalled = false;
}

void GameStateController::mainLoop()
{
	u32 lastFPS = -1;
	while (device->run()) {
		if (stateChangeCalled) {
			stateChange(); //Updates state if the change has been called by one of the controllers
		}

		u32 now = device->getTimer()->getTime();
		switch (state) { //Calls updates from its controllers based on the current state
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