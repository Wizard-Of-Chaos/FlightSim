#include "HUDActiveSelection.h"
#include "SceneManager.h"
#include "GameController.h"
#include "GameStateController.h"
#include <iostream>

HUDActiveSelection::HUDActiveSelection(IGUIElement* root) : HUDElement(root)
{
	type = HUD_ELEM_TYPE::ACTIVE_SELECTION;
	selectGUI = guienv->addImage(stateController->assets.getHUDAsset("neutralSelection"), position2di(0, 0), root);
	name = guienv->addStaticText(L"", rect<s32>(position2di(0, 0), dimension2du(128, 128)), false, false, root);
	name->setOverrideColor(SColor(255, 100, 255, 100));
	name->setOverrideFont(stateController->assets.getFontAsset("HUDFont"));
	name->enableOverrideColor(true);
	crosshair = guienv->addImage(stateController->assets.getHUDAsset("crosshair"), position2di(-200, -200), root);
	selectGUI->setVisible(false);
	name->setVisible(false);
	crosshair->setVisible(false);
}

HUDActiveSelection::~HUDActiveSelection()
{
	selectGUI->remove();
	name->remove();
	crosshair->remove();
}

void HUDActiveSelection::updateElement(EntityId playerId)
{
	auto player = sceneManager->scene.get<PlayerComponent>(playerId);
	auto input = sceneManager->scene.get<InputComponent>(playerId);
	auto playerIrr = sceneManager->scene.get<IrrlichtComponent>(playerId);
	auto sensors = sceneManager->scene.get<SensorComponent>(playerId);
	auto rbc = sceneManager->scene.get<BulletRigidBodyComponent>(playerId);

	ICameraSceneNode* camera = player->camera;

	ISceneCollisionManager* coll = smgr->getSceneCollisionManager();
	line3df ray = input->cameraRay;

	if (!sceneManager->scene.entityInUse(sensors->targetContact)) { //Check to see if the entity still exists
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
					sensors->targetContact = id;
					mouseOverHUD = true;
					break;
				}
			}
			if (mouseOverHUD) break;
		}
		ISceneNode* selection;
		if (mouseOverHUD) selection = sceneManager->scene.get<IrrlichtComponent>(sensors->targetContact)->node;
		else selection = coll->getSceneNodeFromRayBB(ray, ID_IsSelectable);

		if (selection) {
			if (selection->getID() != -1 && selection != playerIrr->node) {
				EntityId id = strToId(selection->getName());
				if(sceneManager->scene.entityInUse(id)) sensors->targetContact = id;

				auto irr = sceneManager->scene.get<IrrlichtComponent>(id);
				std::wstring widestr = std::wstring(irr->name.begin(), irr->name.end());
				name->setText(widestr.c_str());
				selectGUI->setVisible(true);
				name->setVisible(true);
				crosshair->setVisible(true);
			}
		}
		else if (!selection) {
			selectGUI->setVisible(false);
			name->setVisible(false);
			crosshair->setVisible(false);
			sensors->targetContact = INVALID_ENTITY;
		}
	}
	if (sensors->targetContact == INVALID_ENTITY) {
		selectGUI->setVisible(false);
		name->setVisible(false);
		crosshair->setVisible(false);
		return;
	}
	auto irr = sceneManager->scene.get<IrrlichtComponent>(sensors->targetContact);
	if (!irr) {
		sensors->targetContact = INVALID_ENTITY;
		return;
	}
	//Moves around the selection GUI
	position2di selectionPos = coll->getScreenCoordinatesFrom3DPosition(irr->node->getAbsolutePosition(), camera);
	selectionPos.X -= 64;
	selectionPos.Y -= 64;
	selectGUI->setRelativePosition(selectionPos);
	selectionPos.Y -= 16;
	name->setRelativePosition(selectionPos);

	auto targetRBC = sceneManager->scene.get<BulletRigidBodyComponent>(sensors->targetContact);
	auto targetGhost = sceneManager->scene.get<BulletGhostComponent>(sensors->targetContact);
	if (!targetRBC && !targetGhost) return;
	btVector3 velocity = rbc->rigidBody.getLinearVelocity();
	btVector3 forwardTarget;
	if (targetRBC) {
		forwardTarget = targetRBC->rigidBody.getCenterOfMassPosition() + (targetRBC->rigidBody.getLinearVelocity() * .3f);
		forwardTarget += (rbc->rigidBody.getLinearVelocity() * -.3f);
	}
	else if (targetGhost) {
		forwardTarget = irrVecToBt(irr->node->getAbsolutePosition());
	}

	position2di crosshairPos = coll->getScreenCoordinatesFrom3DPosition(btVecToIrr(forwardTarget), camera);
	crosshairPos.X -= 32;
	crosshairPos.Y -= 32;
	crosshair->setRelativePosition(crosshairPos);
}