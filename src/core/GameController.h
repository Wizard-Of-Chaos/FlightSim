#pragma once

#ifndef FLIGHTEVENTRECEIVER_H
#define FLIGHTEVENTRECEIVER_H
#include "BaseHeader.h"

#include "ECS.h"
#include "SceneManager.h"
#include "IrrlichtComponent.h"
#include "InputComponent.h"
#include "GameFunctions.h"
#include "ShipUtils.h"
#include "ObstacleUtils.h"
#include "Scenario.h"
#include "Config.h"

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
#if _DEBUG
		btDebugRenderer rend;
#endif 
		GameController();
		void init();
		void close();

		void update();

		void clearPlayerHUD();

		std::vector<Scenario> usedScenarios; //might need it later?
		Scenario currentScenario;
		void initScenario();
		bool checkRunningScenario();

		GameConfig gameConfig;

	private:
		bool open;

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