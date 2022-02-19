#include "HUDCrosshair.h"
#include "SceneManager.h"
#include "GameController.h"

HUDCrosshair::HUDCrosshair(SceneManager* man) : HUDElement(man)
{
	crosshair = man->controller->guienv->addImage(man->defaults.defaultCrosshairTexture, position2di(0, 0));
}

HUDCrosshair::~HUDCrosshair()
{
	crosshair->remove();
}

void HUDCrosshair::updateElement(SceneManager* manager, PlayerComponent* player, ISceneNode* playerShip, InputComponent* input)
{
	ICameraSceneNode* camera = player->camera;
	ISceneCollisionManager* coll = manager->controller->smgr->getSceneCollisionManager();

	vector3df aheadCrosshairPos = playerShip->getPosition() + getNodeForward(playerShip) * 300.f;
	position2di crosshairPos = coll->getScreenCoordinatesFrom3DPosition(aheadCrosshairPos, camera);
	crosshairPos.X -= 32;
	crosshairPos.Y -= 32;
	crosshair->setRelativePosition(crosshairPos);
}