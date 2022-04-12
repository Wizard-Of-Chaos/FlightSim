#pragma once

#ifndef FLIGHTEVENTRECEIVER_H
#define FLIGHTEVENTRECEIVER_H
#include "BaseHeader.h"

#include "ECS.h"
#include "SceneManager.h"
#include "IrrlichtComponent.h"
#include "InputComponent.h"
#include "GameFunctions.h"
#include "Scenario.h"
#include "Config.h"

class GameStateController;

/*
* The GameController class holds all the necessary information about what's actually going on in the game. It has pointers to the
* various drivers for the game (the irrlicht device, video driver, Irrlicht scene manager, ECS manager, sound engine, etc) and handles
* updates for the game. It is the Keeper of the DT (delta time) which determines how frequently scene and physics updates are called.
* 
* The init() function is used to initialize the game (set up the physics world, irrlicht stuff, some other things), and the close() function
* gets rid of those (such as when you return to the game menus). Many things include the GameController as a pointer - such as the SceneManager class.
*/
class GameController
{
	public:
		bool OnEvent(const SEvent& event);

		IrrlichtDevice* device;
		IVideoDriver* driver;
		ISceneManager* smgr;
		IGUIEnvironment* guienv;
		BulletPhysicsWorld* bWorld;
		ISoundEngine* soundEngine;
		
		GameStateController* stateController;
#if _DEBUG
		btDebugRenderer rend;
#endif 
		GameController(GameStateController* controller);
		void init();
		void close();

		void initDefaultScene();
		void update();

		void clearPlayerHUD();

		std::vector<Scenario> usedScenarios; //might need it later?
		Scenario currentScenario;
		void initScenario();
		bool checkRunningScenario();

		GameConfig gameConfig;

	private:
		bool open;

		//scene management
		SceneManager sceneECS; 
		u32 then;
		f32 accumulator = 0.0f;
		f32 dt = 0.005f;
		f32 t = 0.0f;

		//bullet stuff
		btBroadphaseInterface* broadPhase;
		btDefaultCollisionConfiguration* collisionConfig;
		btCollisionDispatcher* dispatcher;
		btSequentialImpulseConstraintSolver* solver;
		broadCallback* collCb;
		btGhostPairCallback* gPairCb;

		//scenario init and generation
		void killHostilesScenario();
};


#endif