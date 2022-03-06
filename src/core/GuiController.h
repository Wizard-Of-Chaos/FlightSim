#pragma once

#ifndef GUICONTROLLER_H
#define GUICONTROLLER_H
#include "BaseHeader.h"
#include "MenuData.h"
#include <unordered_map>
#include <functional>

class GameStateController;

typedef std::function<bool(const SEvent&)> GuiCallback;

/*
* The GUI controller handles updates for all the menus in the game (start menu, settings menu, death menu, etc).
* It takes in events with the OnEvent call, and holds pointers to any necessary information.
* 
* The big thing it does is contains the active dialogs and a list of dialogs that it can run. For example, if the
* player is currently on the main menu, the main menu will be  the active dialog. Changing menus will call setActiveDialog()
* and other functions. It also has a list of taunts for the death screen (loaded from a text file). The event taken in
* OnEvent gets passed down the chain to the current active dialog.
*/
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

		void setCallback(IGUIElement* elem, GuiCallback callback);

	private:
		GuiDialog* activeDialog;
		MenuData menus;
		std::vector<std::wstring> taunts;
		std::unordered_map<IGUIElement*, GuiCallback> callbacks;
};

#endif