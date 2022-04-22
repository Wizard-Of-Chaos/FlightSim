#pragma once
#ifndef CAMPAIGNMENU_H
#define CAMPAIGNMENU_H
#include "BaseHeader.h"
#include "GuiDialog.h"

enum CAMPAIGN_MENU_ELEMENT
{
	CAMPAIGN_START_BUTTON,
	CAMPAIGN_TO_MENU,
	CAMPAIGN_TO_CONSTRUCTION,
	CAMPAIGN_LOADOUT
};

struct CampaignHUD
{
	IGUIImage* HUDimg;
	IGUIButton* toMenu;
	IGUIStaticText* sector;
	IGUIStaticText* info;
	IGUIButton* constructionButton;
};

struct LoadoutHUD
{
	IGUIImage* img;
	IGUIButton* button;
	bool isOpen = false;
	f32 timer = 0;
};

class GuiCampaignMenu : public GuiDialog
{
	public:
		GuiCampaignMenu() : GuiDialog() {}
		virtual void init();
		bool onStart(const SEvent& event);
		bool onMenu(const SEvent& event);
		bool onLoadout(const SEvent& event);
		bool moveLoadout(f32 dt);
		virtual void show();
	private:
		CampaignHUD hud;
		LoadoutHUD loadout;
		IGUIButton* start;
};
#endif 