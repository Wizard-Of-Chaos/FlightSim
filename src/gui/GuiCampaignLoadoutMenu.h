#pragma once

#ifndef GUICAMPAIGNLOADOUTMENU_H
#define GUICAMPAIGNLOADOUTMENU_H
#include "BaseHeader.h"
#include "GuiDialog.h"
#include "ShipComponent.h"
#include "ECS.h"
#include "LoadoutData.h"

struct ShipSelector
{
	IGUIButton* left;
	IGUIButton* right;
	IGUIStaticText* name;
	s32 curPos = 0;
};

struct WepSelect
{
	IGUIStaticText* bg;
	IGUIButton* select;
	IGUIStaticText* name;
};

class GuiCampaignLoadoutMenu : public GuiDialog
{
	public:
		GuiCampaignLoadoutMenu() : GuiDialog() {}
		virtual void init();
		virtual void show();
		bool onBack(const SEvent& event);
		bool onShipChange(const SEvent& event, bool right);
		bool onShipChangeLeft(const SEvent& event);
		bool onShipChangeRight(const SEvent& event);
		bool onHardpointSelect(const SEvent& event);
		bool onWepSelect(const SEvent& event);
	private:
		void displayShip(ShipInstance& inst);

		WepSelect buildWepSelect(WeaponInfoComponent& wep, position2di pos);
		bool wepSelect(const SEvent& event, ShipInstance& inst);
		bool wepHover(const SEvent& event);

		std::vector<WepSelect> weaponList;
		void displayWeaponList();
		void clearWeaponList();
		u32 currentHardpoint;

		ShipSelector shipSelector;
		IGUIButton* back;
		IGUIImage* wepMenu;
		IGUIStaticText* wepMenuBg;
		IGUIImage* physWepMenu;
		IGUIButton* hardpoints[MAX_HARDPOINTS];
		IGUIButton* physWepHardpoint;
		IGUIImage* curHealth;
		IGUIStaticText* shipInfo;
		IGUIStaticText* shipDesc;
		IGUIStaticText* wepDesc;
};

#endif 