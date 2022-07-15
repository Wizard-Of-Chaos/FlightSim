#pragma once
#ifndef GUILOOTMENU_H
#define GUILOOTMENU_H
#include "BaseHeader.h"
#include "GuiDialog.h"
#include "LoadoutData.h"
#include "WeaponInfoComponent.h"

class GuiLootMenu : public GuiDialog
{
	public:
		GuiLootMenu() : GuiDialog() {}
		virtual void init();
		bool onReturnToCampaign(const SEvent& event);
		bool onShow(f32 dt);
		bool onHide(f32 dt);
		virtual void show();
	private:
		f32 timer;
		IGUIStaticText* loot;
		IGUIImage* screen;
		IGUIButton* returnToCampaign;
		std::vector<ShipInstance*> ships;
		std::vector<WeaponInfoComponent> weps;
};

#endif 