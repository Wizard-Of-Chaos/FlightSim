#include "HUDHealthBar.h"
#include "SceneManager.h"
#include "GameController.h"
#include <iostream>
#include <string>

HUDHealthBar::HUDHealthBar(SceneManager* manager, IGUIElement* root) : HUDElement(manager, root)
{
	rect<s32> screenrect = root->getRelativePosition();
	health = manager->controller->guienv->addImage(manager->defaults.defaultHealthBarTexture, position2di(0, screenrect.getHeight()-64), root);
	healthNum = manager->controller->guienv->addStaticText(L"", rect<s32>(position2di(0, screenrect.getHeight() - 96), dimension2du(300,24)),false, true, root);
	fuel = manager->controller->guienv->addImage(manager->defaults.defaultFuelBarTexture, position2di(0, screenrect.getHeight() - 160), root);
	fuelNum = manager->controller->guienv->addStaticText(L"", rect<s32>(position2di(0, screenrect.getHeight() - 192), dimension2du(300, 24)), false, true, root);
	shield = manager->controller->guienv->addImage(manager->defaults.defaultVelocityBarTexture, position2di(300, screenrect.getHeight() - 64), root);
	shieldNum = manager->controller->guienv->addStaticText(L"", rect<s32>(position2di(300, screenrect.getHeight() - 96), dimension2du(300, 24)), false, true, root);

	fuelNum->setOverrideColor(SColor(255, 200, 200, 200));
	healthNum->setOverrideColor(SColor(255, 200, 200, 200));
	shieldNum->setOverrideColor(SColor(255, 200, 200, 200));
	healthNum->setOverrideFont(manager->defaults.defaultHUDFont);
	fuelNum->setOverrideFont(manager->defaults.defaultHUDFont);
	shieldNum->setOverrideFont(manager->defaults.defaultHUDFont);
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

HUDHealthBar::~HUDHealthBar()
{
	health->remove();
	healthNum->remove();
	fuel->remove();
	fuelNum->remove();
}

void HUDHealthBar::updateElement(SceneManager* manager, EntityId playerId)
{
	auto hpcomp = manager->scene.get<HealthComponent>(playerId);
	auto shields = manager->scene.get<ShieldComponent>(playerId);
	auto player = manager->scene.get<PlayerComponent>(playerId);
	auto ship = manager->scene.get<ShipComponent>(playerId);

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
}
