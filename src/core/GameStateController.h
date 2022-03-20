#pragma once

#ifndef GAMESTATECONTROLLER_H
#define GAMESTATECONTROLLER_H
#include "BaseHeader.h"
#include "GameController.h"
#include "GuiController.h"
#include "Config.h"
#include "LoadoutData.h"
#include <filesystem>

/*
* The states the game can be in - you're in menus, the game is going, the game is paused, or you're dead and waiting to
* return to menus.
*/
enum GAME_STATE
{
	GAME_MENUS = 0,
	GAME_RUNNING = 1,
	GAME_PAUSED = 2,
	GAME_DEAD = 3
};

/*
* The game state controller class holds alllll of the things required for the game to actually function properly. It is set as the
* Irrlicht device's event receiver, and updates its various systems with the OnEvent call. It also includes the current state of the game,
* the main loop, the ability to change states and some logic to determine how to handle those state changes.
* 
* It also holds both the GuiController and GameController class, used for running their respective little worlds. Init should not be called more
* than once, like, ever.
*/



class GameStateController : public IEventReceiver
{
	public:
		virtual bool OnEvent(const SEvent& event);

		IrrlichtDevice* device;
		IVideoDriver* driver;
		ISceneManager* smgr;
		IGUIEnvironment* guienv;
		ISoundEngine* soundEngine;

		GameStateController(IrrlichtDevice* dev, VideoConfig vconf);
		void init();
		void mainLoop();
		void setState(GAME_STATE newState);

		VideoConfig videoConfig;

		std::string playerShip;
		std::string playerWeapons[MAX_HARDPOINTS];

		void loadShipAndWeaponData();
		std::unordered_map<std::string, ShipData*> shipData;
		std::unordered_map<std::string, WeaponData*> weaponData;

	private:
		void stateChange();
		u32 then;
		GAME_STATE state;
		GAME_STATE oldState;
		GameController* gameController;
		GuiController* guiController;
		bool stateChangeCalled = false;
		bool gameInitialized = false;


};

#endif