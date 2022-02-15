#include "HUDActiveSelection.h"
#include "SceneManager.h"
#include "GameController.h"
#include <iostream>

void HUDActiveSelection::updateElement(SceneManager* manager, PlayerComponent* player, ISceneNode* playerShip, InputComponent* input)
{
	ICameraSceneNode* camera = player->camera;

	ISceneCollisionManager* coll = manager->controller->smgr->getSceneCollisionManager();
	line3df ray = coll->getRayFromScreenCoordinates(input->mousePixPosition, camera);

	if (input->rightMouseDown) {
		ISceneNode* selection = coll->getSceneNodeFromRayBB(ray, ID_IsSelectable);
		if (selection) {
			if (selection->getID() != -1 && selection != playerShip) {
				player->activeSelection = selection;
				elem->setVisible(true);
			}
		}
		else if (!selection) {
			elem->setVisible(false);
			player->activeSelection = nullptr;
		}
	}
	if (player->activeSelection == nullptr) {
		elem->setVisible(false);
		return;
	}
	position2di selectionPos = coll->getScreenCoordinatesFrom3DPosition(player->activeSelection->getAbsolutePosition(), camera);
	selectionPos.X -= 64;
	selectionPos.Y -= 64;
	elem->setRelativePosition(selectionPos);
}