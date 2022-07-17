#pragma once
#ifndef GUIWINGMANMENU_H
#define GUIWINGMANMENU_H
#include "BaseHeader.h"
#include "GuiDialog.h"
#include "Campaign.h"

struct wingmanShipPair
{
	IGUIButton* wingman;
	IGUIButton* ship;
};

class GuiWingmanMenu : public GuiDialog
{
	public:
		GuiWingmanMenu() : GuiDialog() {}
		virtual void init();
		bool backToMenu(const SEvent& event);
		bool onWingmanChange(const SEvent& event);
		bool onWingmanSelect(const SEvent& event);
		bool onShipChange(const SEvent& event);
		bool onShipSelect(const SEvent& event);
		bool onWingmanHover(const SEvent& event);
		bool onShipHover(const SEvent& event);

		virtual void show();
	private:
		void displayWingStatus();
		void displayWingmen();
		void displayShips();
		void clearDisplayedList();
		s32 currentSlot;

		IGUIButton* backButton;
		wingmanShipPair wingButtons[3];
		IGUIImage* menuDisplay;
		IGUIStaticText* description;
		
		std::vector<IGUIButton*> buttonList;
};

#endif 