#pragma once
#ifndef CAMPAIGNMENU_H
#define CAMPAIGNMENU_H
#include "BaseHeader.h"
#include "GuiDialog.h"

class CampaignMenu : public GuiDialog
{
	CampaignMenu() : GuiDialog() {}
	virtual void init();
};
#endif 