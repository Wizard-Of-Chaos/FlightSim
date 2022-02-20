#include "HUDCrosshair.h"
#include "SceneManager.h"
#include "GameController.h"

HUDCrosshair::HUDCrosshair(SceneManager* man, IGUIElement* root) : HUDElement(man, root)
{
	crosshair = man->controller->guienv->addImage(man->defaults.defaultCrosshairTexture, position2di(0, 0), root);
}

HUDCrosshair::~HUDCrosshair()
{
	crosshair->remove();
}

void HUDCrosshair::updateElement(SceneManager* manager, EntityId playerId)
{
	auto player = manager->scene.get<PlayerComponent>(playerId);
	auto irr = manager->scene.get<IrrlichtComponent>(playerId);

	ICameraSceneNode* camera = player->camera;
	ISceneCollisionManager* coll = manager->controller->smgr->getSceneCollisionManager();

	vector3df aheadCrosshairPos = irr->node->getPosition() + getNodeForward(irr->node) * 300.f;
	position2di crosshairPos = coll->getScreenCoordinatesFrom3DPosition(aheadCrosshairPos, camera);
	crosshairPos.X -= 32;
	crosshairPos.Y -= 32;
	crosshair->setRelativePosition(crosshairPos);
}