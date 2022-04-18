#pragma once

#ifndef GAMESTATECONTROLLER_H
#define GAMESTATECONTROLLER_H
#include "BaseHeader.h"
#include "GameController.h"
#include "GuiController.h"
#include "Config.h"
#include "LoadoutData.h"
#include "GameAssets.h"
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
* 
* The other thing it does is contain the details for ships and weapons; the actual data associated with them and a prototype component for each.
* These are loaded from .gdat files in attributes/ and stored in two unordered maps for ease of access.
*/
const u32 SHIPID_PLAYER_DEFAULT = 0;
const u32 WEAPONID_INVALID = -1;
const u32 WEAPONID_NONE = 0;


class GameStateController : public IEventReceiver
{
	public:
		virtual bool OnEvent(const SEvent& event);
		GameStateController(VideoConfig vconf);
		void init();
		void mainLoop();
		void setState(GAME_STATE newState);

		VideoConfig videoConfig;

		u32 playerShip;
		u32 playerWeapons[MAX_HARDPOINTS];
		u32 playerPhysWeapon;

		void loadShipAndWeaponData();
		std::unordered_map<u32, ShipData*> shipData;
		std::unordered_map<u32, WeaponData*> weaponData;
		std::unordered_map<u32, WeaponData*> physWeaponData;
		Assets assets;
#if _DEBUG
		void addDebugLine(line3df line) { debugLines.push_back(line); }
#endif 
	private:
		void stateChange();
		u32 then;
		GAME_STATE state;
		GAME_STATE oldState;
		bool stateChangeCalled = false;
		bool gameInitialized = false;
#if _DEBUG
		std::vector<line3df> debugLines;
#endif 

};

#endif