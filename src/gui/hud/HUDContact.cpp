#include "HUDContact.h"
#include "SceneManager.h"
#include "GameController.h"

void HUDContact::updateElement(SceneManager* manager, PlayerComponent* player, ISceneNode* playerShip, InputComponent* input)
{
	if (!manager->scene.entityInUse(contact)) return;
	ICameraSceneNode* camera = player->camera;
	ISceneCollisionManager* coll = manager->controller->smgr->getSceneCollisionManager();
	IrrlichtComponent* irr = manager->scene.get<IrrlichtComponent>(contact);
	if (!irr) return;

	dimension2du screenSize = manager->controller->driver->getScreenSize();
	position2di contactScreenPos = coll->getScreenCoordinatesFrom3DPosition(irr->node->getAbsolutePosition(), camera);
	//if it's off-screen, set basic element to not visible and the marker to visible
	//else do the reverse
	if (contactScreenPos.X < 0 || contactScreenPos.X > screenSize.Width || contactScreenPos.Y < 0 || contactScreenPos.Y > screenSize.Height) {
		//The screenCoordinatesFrom3DPosition sets it to (-10000, -10000) when it's behind the camera.
		//Might be useful to get a dummy camera and invert it? Dunno.
		position2di onscreenPos = contactScreenPos;
		if (onscreenPos.X < 0) {
			onscreenPos.X = 0;
		}
		else if (onscreenPos.X > screenSize.Width) {
			onscreenPos.X = screenSize.Width;
		}

		if (onscreenPos.Y < 0) {
			onscreenPos.Y = 0;
		}
		else if (onscreenPos.Y > screenSize.Height) {
			onscreenPos.Y = screenSize.Height;
		}

		onscreenPos.X -= 32;
		onscreenPos.Y -= 32;
		marker->setRelativePosition(onscreenPos);
		elem->setVisible(false);
		marker->setVisible(true);
	}
	else {
		position2di onscreenPos = contactScreenPos;
		onscreenPos.X -= 32;
		onscreenPos.Y -= 32;
		elem->setRelativePosition(onscreenPos);
		elem->setVisible(true);
		marker->setVisible(false);
	}
}