#include "HUDHealthBar.h"
#include "SceneManager.h"
#include "GameController.h"

HUDHealthBar::HUDHealthBar(SceneManager* manager) : HUDElement(manager)
{
	health = manager->controller->guienv->addImage(manager->defaults.defaultHealthBarTexture, position2di(0, 0));
	health->setScaleImage(false);
}

HUDHealthBar::~HUDHealthBar()
{
	health->remove();
}

void HUDHealthBar::updateElement(SceneManager* manager, PlayerComponent* player, ISceneNode* playerShip, InputComponent* input)
{

}
