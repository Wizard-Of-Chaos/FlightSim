#pragma once

#ifndef FLIGHTEVENTRECEIVER_H
#define FLIGHTEVENTRECEIVER_H
#include "BaseHeader.h"

#include "SystemsHeader.h"
#include "IrrlichtComponent.h"
#include "InputComponent.h"
#include "GameFunctions.h"
#include "ShipUtils.h"
#include "ObstacleUtils.h"
#include "ScenarioUtils.h"
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

typedef std::function<void(flecs::entity)> deathCallback;

struct SoundInstance
{
	flecs::entity_t id;
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

		GameConfig gameConfig;

		std::unordered_map<flecs::entity_t, deathCallback> deathCallbacks;
		std::list<SoundInstance> sounds;

		void registerDeathCallback(flecs::entity id, deathCallback cb) { deathCallbacks[id.id()] = cb; }
		bool hasDeathCallback(flecs::entity id) { return (deathCallbacks.find(id.id()) != deathCallbacks.end()); }

		void registerSoundInstance(flecs::entity id, ISoundSource* snd, f32 volume, f32 radius) {
			if (!id.has<IrrlichtComponent>()) return;
			auto irr = id.get<IrrlichtComponent>();

			ISound* sound = soundEngine->play3D(snd, irr->node->getAbsolutePosition(), false, true);
			if (sound) {
				sound->setVolume(volume);
				sound->setMinDistance(radius);
				sounds.push_back({ id.id(), sound });
				sound->setIsPaused(false);
			}
		}

		flecs::entity playerEntity;
		bool isPlayerAlive;

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
};


#endif
