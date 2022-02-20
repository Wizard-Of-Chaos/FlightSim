#pragma once
#ifndef HUDACTIVESELECTION_H
#define HUDACTIVESELECTION_H
#include "BaseHeader.h"
#include "HUDElement.h"

class HUDActiveSelection : public HUDElement
{
public:
	HUDActiveSelection(SceneManager* man, IGUIElement* root);
	~HUDActiveSelection() override;
	virtual void updateElement(SceneManager* manager, EntityId playerId);
	EntityId activeSelection;
	IGUIStaticText* name;
	IGUIImage* selectGUI;
};

#endif 
