#include "HUDContact.h"
#include "GameController.h"
#include "GameStateController.h"

#include <iostream>

HUDContact::HUDContact(IGUIElement* root, flecs::entity contactId, flecs::entity playerId) : HUDElement(root) 
{
	type = HUD_ELEM_TYPE::CONTACT;

	contact = contactId;
	auto ctctFac = contactId.get<FactionComponent>();
	auto playerFac = playerId.get<FactionComponent>();
	if (playerFac->isHostile(ctctFac)) {
		offscreenMarker = guienv->addImage(assets->getHUDAsset("hostileMarker"), position2di(0, 0), root);
		contactView = guienv->addImage(assets->getHUDAsset("hostileContact"), position2di(0, 0), root);
	}
	else if(playerFac->isFriendly(ctctFac)) {
		offscreenMarker = guienv->addImage(assets->getHUDAsset("friendlyMarker"), position2di(0, 0), root);
		contactView = guienv->addImage(assets->getHUDAsset("friendlyContact"), position2di(0, 0), root);
	}
	else {
		offscreenMarker = guienv->addImage(assets->getHUDAsset("neutralMarker"), position2di(0, 0), root);
		contactView = guienv->addImage(assets->getHUDAsset("neutralContact"), position2di(0, 0), root);
	}
}

HUDContact::~HUDContact()
{
	offscreenMarker->remove();
	contactView->remove();
}

void HUDContact::updateElement(flecs::entity playerId)
{
	auto player = playerId.get<PlayerComponent>();
	flecs::entity ent(game_world->get_world(), contact);
	if (!ent.is_alive()) return;
	ICameraSceneNode* camera = player->camera;
	ISceneCollisionManager* coll = smgr->getSceneCollisionManager();
	auto irr = ent.get<IrrlichtComponent>();
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