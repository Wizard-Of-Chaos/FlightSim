#include "GameController.h"
#include "GameStateController.h"
#include <iostream>
#include <random>

GameController::GameController(GameStateController* controller)
{
	stateController = controller;
	device = controller->device;
	soundEngine = controller->soundEngine;
	smgr = 0;
	guienv = 0;
	driver = 0;
	then = 0;
	bWorld = 0;
}

void GameController::init()
{
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();
	then = device->getTimer()->getTime();

	//bullet init
	broadPhase = new btAxisSweep3(btVector3(-1000, -1000, -1000), btVector3(1000, 1000, 1000));
	collisionConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);
	solver = new btSequentialImpulseConstraintSolver();
	bWorld = new BulletPhysicsWorld(dispatcher, broadPhase, solver, collisionConfig);
	bWorld->setGravity(btVector3(0, 0, 0));

	Scene scene;
	sceneECS = SceneManager(scene, this, bWorld);
	setDefaults(&sceneECS);
}

void GameController::close()
{
	smgr->clear();
	guienv->clear();

	delete broadPhase;
	bWorld->clearObjects();
	delete collisionConfig;
	delete dispatcher;
	delete solver;
	delete bWorld; //this likely leaks some memory

	//delete all the crap in the scenemanager too
	for (ComponentPool* pool : sceneECS.scene.componentPools) {
		delete pool; //pool's closed
	}
}

bool GameController::OnEvent(const SEvent& event)
{
	if (event.EventType == EET_KEY_INPUT_EVENT) {
		for(auto entityId : SceneView<InputComponent>(sceneECS.scene)) {
			InputComponent* input = sceneECS.scene.get<InputComponent>(entityId);
			input->keysDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
			if(event.KeyInput.Key == KEY_KEY_Y && !input->keysDown[KEY_KEY_Y]) {
				input->mouseControlEnabled = !input->mouseControlEnabled;
			}
		}
	}
	if (event.EventType == EET_MOUSE_INPUT_EVENT) {
		for(auto entityId: SceneView<InputComponent>(sceneECS.scene)) {
			InputComponent* input = sceneECS.scene.get<InputComponent>(entityId);
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
		//handle GUI shit here.
	}
	return false;
}

void GameController::initDefaultScene()
{
	EntityId playerId = createDefaultShip(&sceneECS, vector3df(0, 0, 0));
	initializeDefaultPlayer(&sceneECS, playerId);
	initializeDefaultRigidBody(&sceneECS, playerId);
	//initializeNeutralFaction(&sceneECS, playerId);
	initializeDefaultHUD(&sceneECS, playerId);

	EntityId roidId = createDefaultObstacle(&sceneECS, vector3df(0,0,40));
	initializeDefaultRigidBody(&sceneECS, roidId);
	initializeNeutralFaction(&sceneECS, roidId);

	EntityId aiId = createDefaultAIShip(&sceneECS, vector3df(-100, 15, 40));
	initializeHostileFaction(&sceneECS, aiId);

	EntityId friendlyId = createDefaultAIShip(&sceneECS, vector3df(-150, 15, 40));
	initializePlayerFaction(&sceneECS, friendlyId);

	//make the light node an entity as well
	ISceneNode* n = smgr->addLightSceneNode(0, vector3df(0, 5000, 0),
		SColor(200, 255, 180, 180), 20000.f);
	n->setID(ID_IsNotSelectable);

	n = smgr->addBillboardSceneNode(n, dimension2d<f32>(25, 25));
	n->setMaterialFlag(EMF_LIGHTING, false);
	n->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	n->setMaterialTexture(0, driver->getTexture("effects/particlewhite.bmp"));
	n->setID(ID_IsNotSelectable);

	ITexture* sky = driver->getTexture("effects/starskybox.png");
	n = smgr->addSkyBoxSceneNode(sky, sky, sky, sky, sky, sky);
	n->setID(ID_IsNotSelectable);

	device->getCursorControl()->setActiveIcon(ECI_CROSS);
}

void GameController::update()
{
	u32 now = device->getTimer()->getTime();
	f32 delta = (f32)(now - then) / 1000.f;
	if (delta > .25) {
		delta = .25;
	}
	then = now;
	accumulator += delta;
	while (accumulator >= dt) {
		sceneECS.update(dt, delta); //in-game logic and physics
		t += dt;
		accumulator -= dt;
	}

	//interpolate leftover time?
	const f32 alpha = accumulator / dt;
}