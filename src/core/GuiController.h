#pragma once

#ifndef GUICONTROLLER_H
#define GUICONTROLLER_H
#include "BaseHeader.h"
#include "MenuData.h"
#include <unordered_map>
#include <functional>

typedef std::function<bool(const SEvent&)> GuiCallback;
typedef std::function<bool(f32)> AnimationCallback; //should return FALSE when done and TRUE when not done

struct OkPopup
{
	IGUIImage* bg;
	IGUIButton* button;
	IGUIStaticText* title;
	IGUIStaticText* body;
};

struct YesNoPopup
{
	IGUIImage* bg;
	IGUIButton* yes;
	IGUIButton* no;
	IGUIStaticText* title;
	IGUIStaticText* body;
};

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
		GuiController();
		bool OnEvent(const SEvent& event);
		void init();
		void close();
		void update();
		GuiDialog* getActiveDialog() { return activeDialog; }
		void setActiveDialog(MENU_TYPE menu);

		std::wstring getTaunt();
		std::wstring getCongrats();

		void setOkPopup(std::string title, std::string body, std::string button = "Got it");
		void showOkPopup();
		void setYesNoPopup(std::string title, std::string body, GuiCallback yesCb, std::string yes = "Yes", std::string no = "No");
		void showYesNoPopup();
		bool hidePopup(const SEvent& event);

		void setCallback(IGUIElement* elem, GuiCallback callback);
		void removeCallback(IGUIElement* elem);

		void setAnimationCallback(IGUIElement* elem, AnimationCallback cb);
		void setOpenAnimationCallback(GuiDialog* dialog, AnimationCallback cb);
		void setCloseAnimationCallback(GuiDialog* dialog, AnimationCallback cb);
		void callCloseAnimation(GuiDialog* dialog);
		void callOpenAnimation(GuiDialog* dialog);
		void callAnimation(IGUIElement* elem);

		bool initPopupUsed = true;
	private:
		u32 then;
		f32 accumulator = 0.0f;
		f32 dt = 0.005f;
		f32 t = 0.0f;

		bool playingAnimation = false;
		bool switchMenusCalled = false;
		MENU_TYPE menuToSwitch;
		AnimationCallback currentAnimation;

		bool popupActive = false;
		OkPopup okPopup;
		YesNoPopup yesNoPopup;

		GuiDialog* activeDialog;
		std::map<MENU_TYPE, GuiDialog*> menus;
		std::vector<std::wstring> taunts;
		std::vector<std::wstring> congrats;
		std::unordered_map<IGUIElement*, GuiCallback> callbacks;
		std::unordered_map<IGUIElement*, AnimationCallback> animationCallbacks;
		std::unordered_map<GuiDialog*, AnimationCallback> closeAnimationCallbacks;
		std::unordered_map<GuiDialog*, AnimationCallback> openAnimationCallbacks;
};

#endif