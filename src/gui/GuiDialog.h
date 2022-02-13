#pragma once

#ifndef GUIDIALOG_H
#define GUIDIALOG_H
#include "BaseHeader.h"

class GuiController;

class GuiDialog
{
	public:

		GuiDialog(GuiController* controller);
		virtual ~GuiDialog() {}

		//These functions MUST be implemented. How that gets done can vary.
		//Easiest way is to declare a gui element as "root" in your inherited class,
		//then setting up all your other elements as childen of that node.
		//For an example, go check GuiMainMenu.h
		virtual void init() = 0;

		void show();
		void hide();
		//The GUI controller passes events to the currently active dialog and lets them handle it.
		//Such as button clicking! Or mouse movement, if you want to do something odd.
		virtual void handleEvent(const SEvent& event) = 0;
		bool isDialogVisible() { if (root) { return root->isVisible(); } return false; }
	protected:
		//The base size will be set to 960x540. GUI design can assume that you're working for a screen of that size.
		//The elements will automatically scale with the size of the UI - assuming you set that up properly.
		dimension2du baseSize;
		//The root node is an effectively empty node that show and hide gets called on. All following GUI elements
		//need to be set as a child of the root node.
		IGUIElement* root;
		GuiController* guiController;
};

void scaleAlign(IGUIElement* elem);

#endif

