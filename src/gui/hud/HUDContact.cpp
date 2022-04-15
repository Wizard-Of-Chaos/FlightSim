#include "HUDContact.h"
#include "SceneManager.h"
#include "GameController.h"
#include <iostream>

HUDContact::HUDContact(SceneManager* man, IGUIElement* root, EntityId contactId) : HUDElement(man, root) 
{
	type = HUD_ELEM_TYPE::CONTACT;
	offscreenMarker = man->controller->guienv->addImage(man->defaults.defaultContactMarkerTexture, position2di(0, 0), root);
	contactView = man->controller->guienv->addImage(man->defaults.defaultContactTexture, position2di(0, 0), root);
	contact = contactId;
}

HUDContact::~HUDContact()
{
	offscreenMarker->remove();
	contactView->remove();
}

void HUDContact::updateElement(SceneManager* manager, EntityId playerId)
{
	auto player = manager->scene.get<PlayerComponent>(playerId);

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
		offscreenMarker->setRelativePosition(onscreenPos);
		contactView->setVisible(false);
		offscreenMarker->setVisible(true);
	}
	else {
		position2di onscreenPos = contactScreenPos;
		onscreenPos.X -= 32;
		onscreenPos.Y -= 32;
		contactView->setRelativePosition(onscreenPos);
		contactView->setVisible(true);
		offscreenMarker->setVisible(false);
	}
}