#include "HUDHealthBar.h"
#include "GameController.h"
#include "GameStateController.h"

#include <iostream>
#include <string>

HUDResources::HUDResources(IGUIElement* root, flecs::entity id) : HUDElement(root)
{
	IGUIFont* fnt = stateController->assets.getFontAsset("HUDFont");
	SColor overrideClr(255, 200, 200, 200);
	type = HUD_ELEM_TYPE::HEALTH_BAR;
	rect<s32> screenrect = root->getRelativePosition();
	health = guienv->addImage(stateController->assets.getHUDAsset("healthbar"), position2di(0, screenrect.getHeight() - 64), root);
	healthNum = guienv->addStaticText(L"", rect<s32>(position2di(0, screenrect.getHeight() - 96), dimension2du(300,24)),false, true, root);
	fuel = guienv->addImage(stateController->assets.getHUDAsset("fuelbar"), position2di(0, screenrect.getHeight() - 160), root);
	fuelNum = guienv->addStaticText(L"", rect<s32>(position2di(0, screenrect.getHeight() - 192), dimension2du(300, 24)), false, true, root);
	shield = guienv->addImage(stateController->assets.getHUDAsset("velocitybar"), position2di(300, screenrect.getHeight() - 64), root);
	shieldNum = guienv->addStaticText(L"", rect<s32>(position2di(300, screenrect.getHeight() - 96), dimension2du(300, 24)), false, true, root);

	auto shipInfo = id.get<ShipComponent>();

	for (u32 i = 0; i < shipInfo->hardpointCount; ++i) {
		if (!shipInfo->weapons[i].is_alive()) continue;

		auto wepInfo = shipInfo->weapons[i].get<WeaponInfoComponent>();
		if (!wepInfo->usesAmmunition) continue;
		ammoNums[i] = guienv->addStaticText(L"", rect<s32>(position2di(340, screenrect.getHeight() - 16 * (i + 1)), dimension2du(240, 20)), false, true, root);
		ammoNums[i]->setOverrideColor(overrideClr);
		ammoNums[i]->setOverrideFont(fnt);
		ammoNums[i]->setVisible(true);

	}

	fuelNum->setOverrideColor(overrideClr);
	healthNum->setOverrideColor(overrideClr);
	shieldNum->setOverrideColor(overrideClr);
	healthNum->setOverrideFont(fnt);
	fuelNum->setOverrideFont(fnt);
	shieldNum->setOverrideFont(fnt);
	health->setScaleImage(false);
	fuel->setScaleImage(false);
	shield->setScaleImage(false);

	health->setVisible(true);
	healthNum->setVisible(true);
	fuel->setVisible(true);
	fuelNum->setVisible(true);
	shield->setVisible(true);
	shieldNum->setVisible(true);

}

HUDResources::~HUDResources()
{
	health->remove();
	healthNum->remove();
	fuel->remove();
	fuelNum->remove();
	shield->remove();
	shieldNum->remove();
}

void HUDResources::updateElement(flecs::entity playerId)
{
	auto hpcomp = playerId.get<HealthComponent>();
	auto shields = playerId.get<ShieldComponent>();
	auto player = playerId.get<PlayerComponent>();
	auto ship = playerId.get<ShipComponent>();

	std::string hp = fprecis(hpcomp->health, 5);
	std::string maxhp = fprecis(hpcomp->maxHealth, 5);
	std::string hpstr = "HP: " + hp + "/" + maxhp;
	healthNum->setText(wstr(hpstr).c_str());

	std::string fuelstr = "Afterburner: " + fprecis(ship->afterburnerFuel, 5) + "/" + fprecis(ship->maxAfterburnerFuel, 5);
	fuelNum->setText(wstr(fuelstr).c_str());

	std::string shieldstr = "Shields: " + fprecis(shields->shields, 5) + "/" + fprecis(shields->maxShields, 5);
	shieldNum->setText(wstr(shieldstr).c_str());

	rect<s32> screenrect = player->rootHUD->getRelativePosition();
	dimension2du hpsize, fsize, ssize;
	hpsize.set((u32)hpcomp->health / hpcomp->maxHealth * 192, 64); //where 192 is the length of the health bar. Could be changed later
	fsize.set((u32)ship->afterburnerFuel / ship->maxAfterburnerFuel * 192, 64);
	ssize.set((u32)shields->shields / shields->maxShields * 192, 64);
	health->setRelativePosition(rect<s32>(position2di(0, screenrect.getHeight() - 64), hpsize));
	healthNum->setRelativePosition(rect<s32>(position2di(0, screenrect.getHeight() - 96), dimension2du(300, 24)));
	fuel->setRelativePosition(rect<s32>(position2di(0, screenrect.getHeight() - 160), fsize));
	fuelNum->setRelativePosition(rect<s32>(position2di(0, screenrect.getHeight() - 192), dimension2du(300, 24)));
	shield->setRelativePosition(rect<s32>(position2di(300, screenrect.getHeight() - 64), ssize));
	shieldNum->setRelativePosition(rect<s32>(position2di(300, screenrect.getHeight() - 96), dimension2du(300, 24)));

	for (u32 i = 0; i < MAX_HARDPOINTS; ++i) {
		if (!ammoNums[i]) continue;
		auto wepInfo = ship->weapons[i].get<WeaponInfoComponent>();
		if (!wepInfo) continue;
		if (!wepInfo->usesAmmunition) continue;

		std::string ammostr = "Ammunition: " + std::to_string(wepInfo->clip) + "/" + std::to_string(wepInfo->maxClip);
		ammostr += " (" + std::to_string(wepInfo->ammunition) + "/" + std::to_string(wepInfo->maxAmmunition) + ")";
		ammoNums[i]->setText(wstr(ammostr).c_str());
		ammoNums[i]->setRelativePosition(rect<s32>(position2di(550, screenrect.getHeight() - 32 * (i + 1)), dimension2du(240, 20)));
	}
}
