#pragma once

#ifndef GUIDIALOG_H
#define GUIDIALOG_H
#include "BaseHeader.h"
#include "IrrlichtUtils.h"

class GuiController;

class GuiDialog
{
	public:

		GuiDialog(GuiController* controller);
		virtual ~GuiDialog() {}

		//This function MUST be implemented. How that gets done can vary.
		//For an example, go check GuiMainMenu.h
		virtual void init() = 0;
		//The GUI controller passes events to the currently active dialog and lets them handle it.
		//Such as button clicking! Or mouse movement, if you want to do something odd. This function MUST be implemented.
		virtual void handleEvent(const SEvent& event) = 0;

		//These functions show or hide the root node (and thus show or hide all the children).
		void show();
		void hide();
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

#endif

