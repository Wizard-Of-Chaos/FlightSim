#include "HUDContact.h"
#include "SceneManager.h"
#include "GameController.h"
#include <iostream>

HUDContact::HUDContact(IGUIElement* root, EntityId contactId) : HUDElement(root) 
{
	type = HUD_ELEM_TYPE::CONTACT;
	offscreenMarker = guienv->addImage(sceneManager->defaults.defaultContactMarkerTexture, position2di(0, 0), root);
	contactView = guienv->addImage(sceneManager->defaults.defaultContactTexture, position2di(0, 0), root);
	contact = contactId;
}

HUDContact::~HUDContact()
{
	offscreenMarker->remove();
	contactView->remove();
}

void HUDContact::updateElement(EntityId playerId)
{
	auto player = sceneManager->scene.get<PlayerComponent>(playerId);

	if (!sceneManager->scene.entityInUse(contact)) return;
	ICameraSceneNode* camera = player->camera;
	ISceneCollisionManager* coll = smgr->getSceneCollisionManager();
	IrrlichtComponent* irr = sceneManager->scene.get<IrrlichtComponent>(contact);
	if (!irr) return;

	dimension2du screenSize = driver->getScreenSize();
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