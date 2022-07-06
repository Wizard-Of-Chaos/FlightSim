#include "HUDActiveSelection.h"
#include "GameController.h"
#include "GameStateController.h"
#include <iostream>

HUDActiveSelection::HUDActiveSelection(IGUIElement* root) : HUDElement(root)
{
	type = HUD_ELEM_TYPE::ACTIVE_SELECTION;
	selectGUI = guienv->addImage(assets->getHUDAsset("neutralSelection"), position2di(0, 0), root);
	name = guienv->addStaticText(L"", rect<s32>(position2di(0, 0), dimension2du(128, 128)), false, false, root);
	name->setOverrideColor(SColor(255, 100, 255, 100));
	name->setOverrideFont(assets->getFontAsset("HUDFont"));
	name->enableOverrideColor(true);
	crosshair = guienv->addImage(assets->getHUDAsset("crosshair"), position2di(-200, -200), root);
	selectHP = guienv->addImage(assets->getHUDAsset("selectHealth"), position2di(0, 0), root);
	selectSP = guienv->addImage(assets->getHUDAsset("selectShields"), position2di(0, 0), root);
	selectGUI->setVisible(false);
	name->setVisible(false);
	crosshair->setVisible(false);
	selectHP->setVisible(false);
	selectSP->setVisible(false);
}

HUDActiveSelection::~HUDActiveSelection()
{
	selectGUI->remove();
	name->remove();
	crosshair->remove();
	selectHP->remove();
	selectSP->remove();
}

void HUDActiveSelection::updateElement(flecs::entity playerId)
{
	auto player = gameController->getPlayer().get_mut<PlayerComponent>();
	auto input = gameController->getPlayer().get_mut<InputComponent>();
	auto playerIrr = gameController->getPlayer().get_mut<IrrlichtComponent>();
	auto sensors = gameController->getPlayer().get_mut<SensorComponent>();
	auto rbc = gameController->getPlayer().get_mut<BulletRigidBodyComponent>();

	ICameraSceneNode* camera = player->camera;

	ISceneCollisionManager* coll = smgr->getSceneCollisionManager();
	line3df ray = input->cameraRay;

	if (!sensors->targetContact.is_alive()) { //Check to see if the entity still exists
		sensors->targetContact = INVALID_ENTITY;
	}

	if (input->isKeyDown(KEY_KEY_T)) { //Uses the input component to hurl out a ray selecting anything in its path
		bool mouseOverHUD = false;
		for (HUDElement* h : player->HUD) {
			if (h->type != HUD_ELEM_TYPE::CONTACT) continue;

			HUDContact* cont = (HUDContact*)h;
			if (cont->contactView->getAbsolutePosition().isPointInside(input->mousePixPosition)) {
				for (auto [id, c] : player->trackedContacts) {
					if (c != cont) continue;
					sensors->targetContact = flecs::entity(game_world->get_world(), id);
					mouseOverHUD = true;
					break;
				}
			}
			if (mouseOverHUD) break;
		}
		ISceneNode* selection;
		if (mouseOverHUD) selection = sensors->targetContact.get<IrrlichtComponent>()->node;
		else selection = coll->getSceneNodeFromRayBB(ray, ID_IsSelectable);

		if (selection) {
			if (selection->getID() != -1 && selection != playerIrr->node) {
				flecs::entity id = strToId(selection->getName());
				if(id.is_alive()) sensors->targetContact = id;

				auto irr = id.get<IrrlichtComponent>();
				std::wstring widestr = std::wstring(irr->name.begin(), irr->name.end());
				name->setText(widestr.c_str());
				selectGUI->setVisible(true);
				name->setVisible(true);
				crosshair->setVisible(true);
				selectHP->setVisible(true);
				selectSP->setVisible(true);
			}
		}
		else if (!selection) {
			selectGUI->setVisible(false);
			name->setVisible(false);
			crosshair->setVisible(false);
			selectHP->setVisible(false);
			selectSP->setVisible(false);
			sensors->targetContact = INVALID_ENTITY;
		}
	}
	if (sensors->targetContact == INVALID_ENTITY) {
		selectGUI->setVisible(false);
		name->setVisible(false);
		crosshair->setVisible(false);
		selectHP->setVisible(false);
		selectSP->setVisible(false);
		return;
	}
	if (!sensors->targetContact.has<IrrlichtComponent>()) {
		sensors->targetContact = INVALID_ENTITY;
		return;
	}
	auto irr = sensors->targetContact.get<IrrlichtComponent>();
	//Moves around the selection GUI
	position2di selectionPos = coll->getScreenCoordinatesFrom3DPosition(irr->node->getAbsolutePosition(), camera);
	selectionPos.X -= 64;
	selectionPos.Y -= 64;
	selectGUI->setRelativePosition(selectionPos);
	selectionPos.Y -= 16;
	name->setRelativePosition(selectionPos);
	selectionPos.Y += 144;
	position2di hpPos, spPos;
	hpPos = selectionPos;
	selectionPos.Y += 8;
	spPos = selectionPos;

	if (sensors->targetContact.has<ShieldComponent>()) {
		auto targetSP = sensors->targetContact.get<ShieldComponent>();
		dimension2du spSize;
		spSize.set((u32)targetSP->shields / targetSP->maxShields * 128, 8);
		selectSP->setRelativePosition(rect<s32>(spPos, spSize));
	} else {
		selectSP->setVisible(false);
	}
	if (sensors->targetContact.has<HealthComponent>()) {
		auto targetHP = sensors->targetContact.get<HealthComponent>();
		dimension2du hpSize;
		hpSize.set((u32)targetHP->health / targetHP->maxHealth * 128, 8);
		selectHP->setRelativePosition(rect<s32>(hpPos, hpSize));
	} else {
		selectHP->setVisible(false);
	}

	if (!sensors->targetContact.has<BulletRigidBodyComponent>() && !sensors->targetContact.has<BulletGhostComponent>()) return;
	btVector3 velocity = rbc->rigidBody->getLinearVelocity();
	btVector3 forwardTarget;
	if (sensors->targetContact.has<BulletRigidBodyComponent>()) {
		auto targetRBC = sensors->targetContact.get<BulletRigidBodyComponent>();
		forwardTarget = targetRBC->rigidBody->getCenterOfMassPosition() + (targetRBC->rigidBody->getLinearVelocity() * .3f);
		forwardTarget += (rbc->rigidBody->getLinearVelocity() * -.3f);
	}
	else if (sensors->targetContact.has<BulletGhostComponent>()) {
		forwardTarget = irrVecToBt(irr->node->getAbsolutePosition());
	}

	position2di crosshairPos = coll->getScreenCoordinatesFrom3DPosition(btVecToIrr(forwardTarget), camera);
	crosshairPos.X -= 32;
	crosshairPos.Y -= 32;
	crosshair->setRelativePosition(crosshairPos);
}