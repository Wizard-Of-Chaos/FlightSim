#pragma once
#ifndef CAMPAIGNMENU_H
#define CAMPAIGNMENU_H
#include "BaseHeader.h"
#include "GuiDialog.h"

enum CAMPAIGN_MENU_ELEMENT
{
	CAMPAIGN_START_BUTTON
};

class GuiCampaignMenu : public GuiDialog
{
	public:
		GuiCampaignMenu() : GuiDialog() {}
		virtual void init();
		bool onStart(const SEvent& event);
	private:
		IGUIButton* start;
};
#endif 