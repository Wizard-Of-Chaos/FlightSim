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
#include <functional>
#include <list>

/*
* The GameController class holds all the necessary information about what's actually going on in the game. It has pointers to the
* various drivers for the game (the irrlicht device, video driver, Irrlicht scene manager, ECS manager, sound engine, etc) and handles
* updates for the game. It is the Keeper of the DT (delta time) which determines how frequently scene and physics updates are called.
* 
* The init() function is used to initialize the game (set up the physics world, irrlicht stuff, some other things), and the close() function
* gets rid of those (such as when you return to the game menus). Many things include the GameController as a pointer - such as the SceneManager class.
*/

typedef std::function<void(EntityId)> deathCallback;

struct SoundInstance
{
	EntityId id;
	ISound* sound;
};

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

		std::unordered_map<EntityId, deathCallback> deathCallbacks;
		std::list<SoundInstance> sounds;

		void registerDeathCallback(EntityId id, deathCallback cb) { deathCallbacks[id] = cb; }
		bool hasDeathCallback(EntityId id) { return (deathCallbacks.find(id) != deathCallbacks.end()); }

		void registerSoundInstance(EntityId id, ISoundSource* snd, f32 volume, f32 radius) {
			auto irr = sceneManager->scene.get<IrrlichtComponent>(id);
			if (!irr) return;

			ISound* sound = soundEngine->play3D(snd, irr->node->getAbsolutePosition(), false, true);
			sound->setVolume(volume);
			sound->setMinDistance(radius);
			sounds.push_back({ id, sound });
			sound->setIsPaused(false);
		}

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
