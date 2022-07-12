#pragma once
#ifndef GUIWINGMANMENU_H
#define GUIWINGMANMENU_H
#include "BaseHeader.h"
#include "GuiDialog.h"
#include "Campaign.h"

class GuiWingmanMenu : public GuiDialog
{
	public:
		GuiWingmanMenu() : GuiDialog() {}
		virtual void init();
		bool backToMenu(const SEvent& event);
	private:
		IGUIButton* backButton;
};

#endif 