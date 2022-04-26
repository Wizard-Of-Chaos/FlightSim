#include "GameController.h"
#include "GameStateController.h"
#include "ObstacleUtils.h"

#include <iostream>
#include <random>

GameController::GameController()
{
	open = false;
	gameConfig.loadConfig("cfg/gameconfig.gdat");
	gameConfig.saveConfig("cfg/gameconfig.gdat");
}

void GameController::update()
{
	u32 now = device->getTimer()->getTime();
	f32 delta = (f32)(now - then) / 1000.f;
	if (delta > .25) { //If the delta is too big, it's .25.
		delta = .25;
	}
	then = now;
	accumulator += delta;
	while (accumulator >= dt) {
		sceneManager->update(dt, delta); //in-game logic and physics
		t += dt;
		accumulator -= dt;
	}
	if (checkRunningScenario()) {
		stateController->setState(GAME_DEAD);
	}

	//interpolate leftover time?
	const f32 alpha = accumulator / dt;
}

void GameController::init()
{
	if (open) return;

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();
	then = device->getTimer()->getTime();

	//Scenario defScenario(SCENARIO_KILL_HOSTILES, SCENENV_ASTEROID_FIELD, 1, vector3df(0, 0, -50), vector3df(10, 20, 80));
	currentScenario = stateController->campaign.currentScenario;

	//bullet init
	broadPhase = new bt32BitAxisSweep3(btVector3(-100000, -100000, -100000), btVector3(100000, 100000, 100000));
	collisionConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);
	solver = new btSequentialImpulseConstraintSolver();
	gPairCb = new btGhostPairCallback();
	bWorld = new BulletPhysicsWorld(dispatcher, broadPhase, solver, collisionConfig);
	bWorld->setGravity(btVector3(0, 0, 0));
	bWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(gPairCb);
#if _DEBUG
	rend.setController(stateController);
	rend.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	bWorld->setDebugDrawer(&rend);
#endif 
	Scene scene;
	sceneManager = new SceneManager(scene); //Sets up the ECS scene

	collCb = new broadCallback();
	bWorld->getPairCache()->setOverlapFilterCallback(collCb);

	open = true;
}

void GameController::close()
{
	if (!open) return;

	clearPlayerHUD();
	smgr->clear();
	bWorld->clearObjects();
	delete broadPhase;
	delete collisionConfig;
	delete dispatcher;
	delete solver;
	delete bWorld; //this likely leaks some memory
	delete collCb;
	delete gPairCb;
	//delete all the crap in the scenemanager too
	for (ComponentPool* pool : sceneManager->scene.componentPools) {
		delete pool; //pool's closed
	}
	delete sceneManager;
	sounds.clear();
	stateController->assets.clearLoadedGameAssets();
	open = false;
}

void GameController::clearPlayerHUD()
{
	for (auto id : SceneView<PlayerComponent>(sceneManager->scene)) {
		auto player = sceneManager->scene.get<PlayerComponent>(id);
		for (HUDElement* hud : player->HUD) {
			delete hud;
		}
		player->rootHUD->remove();
	}
}

bool GameController::OnEvent(const SEvent& event)
{
	if (event.EventType == EET_KEY_INPUT_EVENT) {
		for(auto entityId : SceneView<InputComponent>(sceneManager->scene)) { //Passes key input to the input components
			InputComponent* input = sceneManager->scene.get<InputComponent>(entityId);
			input->keysDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
			if(event.KeyInput.Key == KEY_KEY_Y && !input->keysDown[KEY_KEY_Y]) {
				input->mouseControlEnabled = !input->mouseControlEnabled;
			}
			if (event.KeyInput.Key == KEY_KEY_U && !input->keysDown[KEY_KEY_U]) {
				input->safetyOverride = !input->safetyOverride;
			}
		}
	}
	if (event.EventType == EET_MOUSE_INPUT_EVENT) {
		for(auto entityId: SceneView<InputComponent>(sceneManager->scene)) { //Passes mouse input to the input components
			InputComponent* input = sceneManager->scene.get<InputComponent>(entityId);
			switch(event.MouseInput.Event) {
			case EMIE_LMOUSE_PRESSED_DOWN:
				input->leftMouseDown = true;
				break;
			case EMIE_LMOUSE_LEFT_UP:
				input->leftMouseDown = false;
				break;
			case EMIE_RMOUSE_PRESSED_DOWN:
				input->rightMouseDown = true;
				break;
			case EMIE_RMOUSE_LEFT_UP:
				input->rightMouseDown = false;
				break;
			case EMIE_MOUSE_MOVED:
				input->mousePixPosition.X = event.MouseInput.X;
				input->mousePixPosition.Y = event.MouseInput.Y;
				input->mousePosition.X = (event.MouseInput.X - ((f32)driver->getScreenSize().Width * .5f)) / ((f32)driver->getScreenSize().Width * .5f);
				input->mousePosition.Y = (event.MouseInput.Y - ((f32)driver->getScreenSize().Height * .5f)) / ((f32)driver->getScreenSize().Height * .5f);
				break;
			default:
				break;
			}
		}

	}
	if (event.EventType == EET_GUI_EVENT) {
		//handle GUI events here, but there probably aren't any that the HUD itself doesn't handle
	}
	return false;
}

bool GameController::checkRunningScenario()
{
	for (u32 i = 0; i < currentScenario.objectiveCount; ++i) {
		if (currentScenario.objectives[i] != INVALID_ENTITY && !sceneManager->scene.entityInUse(currentScenario.objectives[i])) {
			currentScenario.objectives[i] = INVALID_ENTITY;
			--currentScenario.activeObjectiveCount;
		}
	}
	if (currentScenario.activeObjectiveCount == 0) return true;
	return false;
}

void GameController::initScenario()
{
	std::cout << "Loading scenario...\n";

	std::cout << "scenario type: " << currentScenario.type << ", scenario environment: " << currentScenario.environment << ", objective count: " << currentScenario.objectiveCount;
	std::cout << "\n player start: " << currentScenario.playerStartPos.X << " " << currentScenario.playerStartPos.Y << " " << currentScenario.playerStartPos.Z;
	std::cout << "\n enemy start: " << currentScenario.enemyStartPos.X << " " << currentScenario.enemyStartPos.Y << " " << currentScenario.enemyStartPos.Z;
	std::cout << "\n num obstacles: " << currentScenario.obstaclePositions.size() << std::endl;
	buildScenario(currentScenario);

	device->getCursorControl()->setActiveIcon(ECI_CROSS);
	std::cout << "Scenario loaded. \n";
}