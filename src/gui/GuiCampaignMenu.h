#pragma once
#ifndef CAMPAIGNMENU_H
#define CAMPAIGNMENU_H
#include "BaseHeader.h"
#include "GuiDialog.h"
#include "Campaign.h"

enum CAMPAIGN_MENU_ELEM
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

enum CAMPAIGN_MENU_ELEM_SCEN_SELECT
{
	OPT1,
	OPT2,
	OPT3,
	OPT4
};

struct CampaignHUD
{
	IGUIImage* HUDimg;
	IGUIImage* orbiting;
	IGUIImage* shipSprite;
	IGUIImage* sectorHeader;
	IGUIButton* scenarioSelects[NUM_SCENARIO_OPTIONS];
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
	u32 showing;
	IGUIImage* launch;
	IGUIButton* launchButton;
	IGUIImage* areadesc;
	IGUIStaticText* name;
	IGUIStaticText* desc;

	f32 timer1 = 0;
	f32 timer2 = 0;
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
		bool moveSectorInfo(f32 dt);
		bool onShowSectorInfo(const SEvent& event);
		virtual void show();
	private:
		bool sectorInfoShowing;
		CampaignHUD hud;
		LoadoutHUD loadout;
		ScenarioHUD scenariohud;
};
#endif 