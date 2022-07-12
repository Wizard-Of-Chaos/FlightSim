#pragma once
#ifndef CAMPAIGNMENU_H
#define CAMPAIGNMENU_H
#include "BaseHeader.h"
#include "GuiDialog.h"
#include "Campaign.h"

//Enum for campaign elements
enum CAMPAIGN_MENU_ELEM
{
	CAMPAIGN_START_BUTTON,
	CAMPAIGN_TO_MENU,
	CAMPAIGN_TO_CONSTRUCTION,
	CAMPAIGN_LOADOUT,

};

//Holds the data for the basic HUD; showing all the scenario buttons, the little ship sprites and the left half of the HUD.
struct CampaignHUD
{
	IGUIImage* HUDimg;
	IGUIImage* orbiting;
	IGUIImage* shipSprite;
	IGUIButton* advance;
	IGUIImage* sectorHeader;
	IGUIButton* scenarioSelects[NUM_SCENARIO_OPTIONS];
	IGUIButton* toMenu;
	IGUIStaticText* sector;
	IGUIStaticText* info;
	IGUIButton* constructionButton;

	f32 timer1 = 0;
	f32 timer2 = 0;
};

//Holds the data for the loadout menu drop-down (drop-up?).
struct LoadoutHUD
{
	IGUIImage* img;
	IGUIButton* button;
	IGUIButton* toLoadoutMenu;
	IGUIButton* wingmanButton;
	bool isOpen = false;
	f32 timer = 0;
};

//Holds the data for showing and launching the currently selected scenario.
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

/*
* The GuiCampaignMenu has the data and the functions to handle the campaign interface. It allows you to select a scenario from a list,
* shows you some basic info about the scenario, and it will allow you to mess with your current "inventory" as well as loadouts for yourself
* and your wingmen.
*/
class GuiCampaignMenu : public GuiDialog
{
	public:
		GuiCampaignMenu() : GuiDialog() {}
		virtual void init();
		bool onStart(const SEvent& event);
		bool onMenu(const SEvent& event);
		bool onLoadout(const SEvent& event);
		bool onWingman(const SEvent& event);
		bool onLoadoutMenuSelect(const SEvent& event);
		bool moveLoadout(f32 dt);
		bool moveSectorInfo(f32 dt);
		bool onShowSectorInfo(const SEvent& event);

		bool onAdvance(const SEvent& event);
		bool advanceConfirm(const SEvent& event);
		bool moveAdvance(f32 dt);

		virtual void show();
	private:
		bool sectorInfoShowing;

		CampaignHUD hud;
		LoadoutHUD loadout;
		ScenarioHUD scenariohud;
};
#endif 