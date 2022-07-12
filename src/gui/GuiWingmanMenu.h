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
	private:
		IGUIButton* backButton;
		wingmanShipPair wingButtons[3];
		IGUIImage* menuDisplay;
		IGUIStaticText* description;
};

#endif 