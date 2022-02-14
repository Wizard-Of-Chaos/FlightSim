#include "HUDCrosshair.h"
#include "SceneManager.h"
#include "GameController.h"

void HUDCrosshair::updateElement(SceneManager* manager, PlayerComponent* player, ISceneNode* playerShip, InputComponent* input)
{
	ICameraSceneNode* camera = player->camera;
	ISceneCollisionManager* coll = manager->controller->smgr->getSceneCollisionManager();

	vector3df aheadCrosshairPos = playerShip->getPosition() + getNodeForward(playerShip) * 300.f;
	position2di crosshairPos = coll->getScreenCoordinatesFrom3DPosition(aheadCrosshairPos, camera);
	crosshairPos.X -= 32;
	crosshairPos.Y -= 32;
	elem->setRelativePosition(crosshairPos);
}