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
	CAMPAIGN_LOADOUT,
	CAMPAIGN_SCENARIO_OPT1,
	CAMPAIGN_SCENARIO_OPT2,
	CAMPAIGN_SCENARIO_OPT3,
	CAMPAIGN_SCENARIO_OPT4,
};

struct CampaignHUD
{
	IGUIImage* HUDimg;
	IGUIImage* orbiting;
	IGUIImage* shipSprite;
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

struct ScenarioHUD
{
	IGUIImage* launch;
	IGUIButton* launchButton;
	IGUIImage* areadesc;
	IGUIStaticText* name;
	IGUIStaticText* desc;
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
		ScenarioHUD scenariohud;

		IGUIButton* start;
};
#endif 