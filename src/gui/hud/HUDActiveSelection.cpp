#include "HUDActiveSelection.h"
#include "SceneManager.h"
#include "GameController.h"
#include <iostream>

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
				elem->setVisible(true);
			}
		}
		else if (!selection) {
			elem->setVisible(false);
			activeSelection = INVALID_ENTITY;
		}
	}
	if (activeSelection == INVALID_ENTITY) {
		elem->setVisible(false);
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
	elem->setRelativePosition(selectionPos);
}