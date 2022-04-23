#include "GameStateController.h"

#include <iostream>

GameStateController::GameStateController(VideoConfig vconf)
{
	std::srand(time(NULL));
	gameInitialized = false;
	videoConfig = vconf;
}

void GameStateController::init()
{
	std::cout << "Starting game... \n";
	device->setEventReceiver(stateController);
	guienv->setUserEventReceiver(stateController);
	then = device->getTimer()->getTime();
	state = GAME_MENUS; //Initial state
	driver->setMinHardwareBufferVertexCount(0);
	driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, true);

	playerShip = 0;
	playerWeapons[0] = 3;
	playerWeapons[1] = 3;
	for (int i = 2; i < MAX_HARDPOINTS; ++i) {
		playerWeapons[i] = WEAPONID_INVALID;
	}
	playerPhysWeapon = 1;

	loadShipAndWeaponData();
	soundEngine = createIrrKlangDevice();

	assets.setFilenames();

	auto music = soundEngine->play2D(assets.getSoundAsset("menuMusic"), true, true); //Todo: call this somewhere else
	music->setVolume(.3f);
	music->setIsPaused(false);
	gameController = new GameController;

	guiController = new GuiController;
	guiController->init();

	IGUIFont* defaultFont = assets.getFontAsset("defaultFont");
	if (defaultFont) {
		guienv->getSkin()->setFont(defaultFont);
	}
	IGUIFont* tooltipDefaultFont = assets.getFontAsset("defaultTooltipFont");
	if (tooltipDefaultFont) {
		guienv->getSkin()->setFont(tooltipDefaultFont, EGDF_TOOLTIP);
	}
	guienv->getSkin()->setColor(EGDC_BUTTON_TEXT, SColor(255, 140, 250, 255));
	std::cout << "Game initialized!\n";
	gameInitialized = true;
}

void GameStateController::loadShipAndWeaponData()
{
	std::string basepath = "attributes/";
	std::string weaponpath = basepath + "weapons/";
	std::string shippath = basepath + "ships/";
	std::string hullpath = basepath + "hulls/";
	std::string obstpath = basepath + "obstacles/";

	gvReader in;
	std::cout << "Loading ships... \n";
	for (const auto& file : std::filesystem::directory_iterator(shippath)) {
		u32 id = loadShipData(file.path().string(), in);
		if (id != -1) {
			btConvexHullShape hull;
			std::string fname = hullpath + shipData[id]->name + ".bullet";
			if (loadHull(fname, hull)) {
				std::cout << "Hull loaded for " << file.path().string() << ".\n";
				shipData[id]->collisionShape = hull;
			}
			else {
				std::cout << "Could not load hull for " << file.path().string() << ". Building hull... ";
				IMesh* mesh = smgr->getMesh(shipData[id]->shipMesh.c_str());
				shipData[id]->collisionShape = createCollisionShapeFromMesh(mesh);
				saveHull(fname, shipData[id]->collisionShape);
				smgr->getMeshCache()->removeMesh(mesh);
				std::cout << "Done. New hull saved to " << fname << ".\n";
			}
		}
		in.clear();
	}
	std::cout << "Done loading ships. \nLoading weapons... \n";
	for (const auto& file : std::filesystem::directory_iterator(weaponpath)) {
		loadWeaponData(file.path().string(), in);
		in.clear();
	}
	std::cout << "Done loading weapons. \nLoading obstacles... \n";
	for (const auto& file : std::filesystem::directory_iterator(obstpath)) {
		u32 id = loadObstacleData(file.path().string(), in);
		in.clear();
		if (id == -1) continue;
		ObstacleData* data = obstacleData[id];
		if (data->type != GAS_CLOUD) {
			btConvexHullShape hull;
			std::string fname = hullpath + data->name + ".bullet";
			if (loadHull(fname, hull)) {
				std::cout << "Loaded hull for " << file.path().string() << ".\n";
				data->shape = hull;
			} else {
				std::cout << "Could not load hull for " << file.path().string() << ". Building hull... ";
				IMesh* mesh = smgr->getMesh(data->obstacleMesh.c_str());
				data->shape = createCollisionShapeFromMesh(mesh);
				saveHull(fname, data->shape);
				smgr->getMeshCache()->removeMesh(mesh);
				std::cout << "Done. New hull saved to " << fname << ".\n";
			}
		}
	}
	std::cout << "Number of weapons: " << weaponData.size() << std::endl;
	std::cout << "Number of physics weapons: " << physWeaponData.size() << std::endl;
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
		gameController->initScenario();
		//gameController->initDefaultScene();
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
#ifdef _DEBUG
				bWorld->debugDrawWorld();
#endif
				break;
			case GAME_PAUSED:
				guiController->update();
				break;
			case GAME_DEAD:
				guiController->update();
				break;
		}
		then = now;
#if _DEBUG
		driver->setTransform(ETS_WORLD, IdentityMatrix);
		SMaterial material;
		material.Lighting = false;
		material.BackfaceCulling = false;
		material.ZBuffer = ECFN_ALWAYS;
		driver->setMaterial(material);
#endif 
		driver->beginScene(true, true, SColor(255, 20, 20, 20));
		smgr->drawAll();
		guienv->drawAll();
#if _DEBUG
		for (line3df line : debugLines) {
			driver->draw3DLine(line.start, line.end);
		}
		debugLines.clear();
#endif 
		/*
		if (now - then > 100) {
			driver->runAllOcclusionQueries(false);
			driver->updateAllOcclusionQueries();
		}
		*/
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