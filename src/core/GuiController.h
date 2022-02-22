#pragma once

#ifndef GUICONTROLLER_H
#define GUICONTROLLER_H
#include "BaseHeader.h"
#include "MenuData.h"

class GameStateController;

class GuiController
{
	public:
		GuiController(GameStateController* controller);
		bool OnEvent(const SEvent& event);
		IrrlichtDevice* device;
		IVideoDriver* driver;
		ISceneManager* smgr;
		IGUIEnvironment* guienv;
		ISoundEngine* soundEngine;

		GameStateController* stateController;
		void init();
		void close();
		void menuCleanup();
		void update();
		GuiDialog* getActiveDialog() { return activeDialog; }
		void setActiveDialog(MENU_TYPE menu);
		std::wstring getTaunt();

	private:
		GuiDialog* activeDialog;
		MenuData menus;
		std::vector<std::wstring> taunts;
		//to-do list: add hookups to store the various menus as well as change the active dialog
};

#endif