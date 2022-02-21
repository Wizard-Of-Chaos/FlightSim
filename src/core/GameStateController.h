#pragma once

#ifndef GAMESTATECONTROLLER_H
#define GAMESTATECONTROLLER_H
#include "BaseHeader.h"
#include "GameController.h"
#include "GuiController.h"

enum GAME_STATE
{
	GAME_MENUS = 0,
	GAME_RUNNING = 1,
	GAME_PAUSED = 2,
	GAME_DEAD = 3
};

class GameStateController : public IEventReceiver
{
	public:
		virtual bool OnEvent(const SEvent& event);

		IrrlichtDevice* device;
		IVideoDriver* driver;
		ISceneManager* smgr;
		IGUIEnvironment* guienv;
		ISoundEngine* soundEngine;

		GameStateController(IrrlichtDevice* dev);
		void init();
		void mainLoop();
		void setState(GAME_STATE newState);
	private:
		void stateChange();
		u32 then;
		GAME_STATE state;
		GAME_STATE oldState;
		GameController* gameController;
		GuiController* guiController;
		bool stateChangeCalled = false;



};

#endif