#include "HUDActiveSelection.h"
#include "SceneManager.h"
#include "GameController.h"
#include <iostream>

HUDActiveSelection::HUDActiveSelection(SceneManager* man) : HUDElement(man)
{
	selectGUI = man->controller->guienv->addImage(man->defaults.defaultSelectionTexture, position2di(0, 0));
	name = man->controller->guienv->addStaticText(L"", rect<s32>(position2di(0, 0), dimension2du(128, 128)));
	name->setOverrideColor(SColor(255, 255, 255, 255));
	name->enableOverrideColor(true);
	activeSelection = INVALID_ENTITY;
	selectGUI->setVisible(false);
	name->setVisible(false);
}

HUDActiveSelection::~HUDActiveSelection()
{
	selectGUI->remove();
	name->remove();
}

void HUDActiveSelection::updateElement(SceneManager* manager, PlayerComponent* player, ISceneNode* playerShip, InputComponent* input)
{
	ICameraSceneNode* camera = player->camera;

	ISceneCollisionManager* coll = manager->controller->smgr->getSceneCollisionManager();
	line3df ray = coll->getRayFromScreenCoordinates(input->mousePixPosition, camera);

	if (!manager->scene.entityInUse(activeSelection)) {
		activeSelection = INVALID_ENTITY;
	}

	if (input->rightMouseDown) {
		ISceneNode* selection = coll->getSceneNodeFromRayBB(ray, ID_IsSelectable);
		if (selection) {
			if (selection->getID() != -1 && selection != playerShip) {
				EntityId id = strToId(selection->getName());
				if(manager->scene.entityInUse(id)) activeSelection = id;
				auto irr = manager->scene.get<IrrlichtComponent>(id);
				std::wstring widestr = std::wstring(irr->name.begin(), irr->name.end());
				name->setText(widestr.c_str());
				selectGUI->setVisible(true);
				name->setVisible(true);
			}
		}
		else if (!selection) {
			selectGUI->setVisible(false);
			name->setVisible(false);
			activeSelection = INVALID_ENTITY;
		}
	}
	if (activeSelection == INVALID_ENTITY) {
		selectGUI->setVisible(false);
		name->setVisible(false);
		return;
	}
	auto irr = manager->scene.get<IrrlichtComponent>(activeSelection);
	if (!irr) {
		activeSelection = INVALID_ENTITY;
		return;
	}
	position2di selectionPos = coll->getScreenCoordinatesFrom3DPosition(irr->node->getAbsolutePosition(), camera);
	selectionPos.X -= 64;
	selectionPos.Y -= 64;
	selectGUI->setRelativePosition(selectionPos);
	selectionPos.Y -= 16;
	name->setRelativePosition(selectionPos);
}