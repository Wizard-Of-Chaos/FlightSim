#pragma once
#ifndef HUDACTIVESELECTION_H
#define HUDACTIVESELECTION_H
#include "BaseHeader.h"
#include "HUDElement.h"

class HUDActiveSelection : public HUDElement
{
public:
	HUDActiveSelection(SceneManager* man);
	~HUDActiveSelection() override;
	virtual void updateElement(SceneManager* manager, PlayerComponent* player, ISceneNode* playerShip, InputComponent* input);
	EntityId activeSelection;
	IGUIStaticText* name;
	IGUIImage* selectGUI;
};

#endif 
