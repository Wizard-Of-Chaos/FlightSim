#pragma once
#ifndef HUDACTIVESELECTION_H
#define HUDACTIVESELECTION_H
#include "BaseHeader.h"
#include "HUDElement.h"

/*
* This class holds the currently active selection for the player. It updates where it is on the screen
* and holds information to display itself and the name of the given entity selected. When the entity is removed
* (i.e. you kill it) it hides itself and stops updating.
*/
class HUDActiveSelection : public HUDElement
{
public:
	HUDActiveSelection(IGUIElement* root);
	~HUDActiveSelection() override;
	virtual void updateElement(flecs::entity playerId);

	IGUIStaticText* name;
	IGUIImage* selectGUI;
	IGUIImage* crosshair;
	IGUIImage* selectHP;
	IGUIImage* selectSP;
};

#endif 
