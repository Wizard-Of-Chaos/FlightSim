#pragma once
#ifndef GUILOOTMENU_H
#define GUILOOTMENU_H
#include "BaseHeader.h"
#include "GuiDialog.h"

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
};

#endif 