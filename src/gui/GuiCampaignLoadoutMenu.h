#pragma once

#ifndef GUICAMPAIGNLOADOUTMENU_H
#define GUICAMPAIGNLOADOUTMENU_H
#include "BaseHeader.h"
#include "GuiDialog.h"
#include "ShipComponent.h"
#include "ECS.h"

struct ShipSelector
{
	IGUIButton* left;
	IGUIButton* right;
	IGUIStaticText* name;
};

class GuiCampaignLoadoutMenu : public GuiDialog
{
	public:
		GuiCampaignLoadoutMenu() : GuiDialog() {}
		virtual void init();
		virtual void show();
		bool onBack(const SEvent& event);

	private:
		IGUIButton* back;
		IGUIImage* wepMenu;
		IGUIImage* physWepMenu;
		IGUIButton* hardpoints[MAX_HARDPOINTS];
		IGUIButton* physWepHardpoint;
		IGUIImage* curHealth;
		IGUIStaticText* shipInfo;
		IGUIStaticText* shipDesc;
		IGUIStaticText* wepDesc;
};

#endif 