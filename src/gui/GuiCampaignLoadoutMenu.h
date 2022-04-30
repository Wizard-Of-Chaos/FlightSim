#pragma once
#ifndef GUICAMPAIGNLOADOUTMENU_H
#define GUICAMPAIGNLOADOUTMENU_H
#include "BaseHeader.h"
#include "GuiDialog.h"

class GuiController;
class GameController;
class GameStateController;

struct GuiCampaignLoadoutMenu : public GuiDialog
{
	public:
		GuiCampaignLoadoutMenu() : GuiDialog() {}
		virtual void init();
		virtual void show();
		bool onBack(const SEvent& event);
	private:
		IGUIButton* backToCampaign;
		IGUIListBox* ships;
};

#endif 