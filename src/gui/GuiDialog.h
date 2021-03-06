#pragma once

#ifndef GUIDIALOG_H
#define GUIDIALOG_H
#include "BaseHeader.h"
#include "IrrlichtUtils.h"

/*
* The base class for all GUI dialogs. Includes a show/hide function as well as a root GUI element and a pointer to the controller.
* All buttons and text and whatnot NEEDS to have the root GUI element as a parent, so the dialog can be shown or hidden.
*/
class GuiDialog
{
	public:

		GuiDialog();

		//This function MUST be implemented. How that gets done can vary.
		//For an example, go check GuiMainMenu.h
		virtual void init() = 0;

		//These functions show or hide the root node (and thus show or hide all the children).
		virtual void show();
		virtual void hide();
		bool isDialogVisible() { if (root) { return root->isVisible(); } return false; }
		IGUIElement* getRoot() { return root; }
	protected:
		//The base size will be set to 960x540. GUI design can assume that you're working for a screen of that size.
		//The elements will automatically scale with the size of the UI - assuming you set that up properly.
		dimension2du baseSize;
		//The root node is an effectively empty node that show and hide gets called on. All following GUI elements
		//need to be set as a child of the root node.
		IGUIElement* root;
};

#endif

