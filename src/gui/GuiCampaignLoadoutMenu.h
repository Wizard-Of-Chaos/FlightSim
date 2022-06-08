#pragma once

#ifndef GUICAMPAIGNLOADOUTMENU_H
#define GUICAMPAIGNLOADOUTMENU_H
#include "BaseHeader.h"
#include "GuiDialog.h"
#include "ShipComponent.h"
#include "LoadoutData.h"

struct ShipSelector
{
	IGUIButton* left;
	IGUIButton* right;
	IGUIStaticText* name;
	IGUIButton* useShip;
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
		bool onPhysWepSelect(const SEvent& event);
		bool onUseShip(const SEvent& event);
	private:
		ShipInstance& getCurShip();
		void displayShip(ShipInstance& inst);

		WepSelect buildWepSelect(WeaponInfoComponent& wep, position2di pos, bool phys);
		bool wepSelect(const SEvent& event, ShipInstance& inst, std::vector<WeaponInfoComponent>& list);
		bool physWepSelect(const SEvent& event, ShipInstance& inst, std::vector<WeaponInfoComponent>& list);
		bool wepHover(const SEvent& event);

		bool onReload(const SEvent& event);
		bool onRepair(const SEvent& event);

		std::vector<WepSelect> weaponList;
		std::vector<WepSelect> physWeaponList;
		void displayWeaponList();
		void displayPhysWeaponList();
		void clearWeaponList();
		void displayCarrierInfo();

		u32 currentHardpoint;

		ShipSelector shipSelector;
		IGUIButton* back;
		IGUIImage* wepMenu;
		IGUIStaticText* wepMenuBg;
		IGUIImage* physWepMenu;
		IGUIStaticText* physWepMenuBg;
		IGUIButton* hardpoints[MAX_HARDPOINTS];
		IGUIButton* reloadHardpoints[MAX_HARDPOINTS];
		IGUIButton* repair;
		IGUIButton* physWepHardpoint;
		IGUIImage* curHealth;
		IGUIStaticText* shipInfo;
		IGUIStaticText* wepDesc;

		IGUIImage* carrierInfoPanel;
		IGUIStaticText* carrierInfo;
};

#endif 