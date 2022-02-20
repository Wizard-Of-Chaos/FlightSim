#include "HUDHealthBar.h"
#include "SceneManager.h"
#include "GameController.h"
#include <iostream>
#include <string>
#include <codecvt>
#include <sstream>

HUDHealthBar::HUDHealthBar(SceneManager* manager, IGUIElement* root) : HUDElement(manager, root)
{
	rect<s32> screenrect = root->getRelativePosition();
	health = manager->controller->guienv->addImage(manager->defaults.defaultHealthBarTexture, position2di(0, screenrect.getHeight()-64), root);
	healthNum = manager->controller->guienv->addStaticText(L"", rect<s32>(position2di(0, screenrect.getHeight() - 96), dimension2du(300,24)),false, true, root);
	healthNum->setOverrideColor(SColor(255, 200, 200, 200));
	health->setScaleImage(false);
	health->setAlignment(EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
	healthNum->setAlignment(EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
	health->setVisible(true);
	healthNum->setVisible(true);
}

HUDHealthBar::~HUDHealthBar()
{
	health->remove();
}

void HUDHealthBar::updateElement(SceneManager* manager, EntityId playerId)
{
	auto hpcomp = manager->scene.get<HealthComponent>(playerId);
	auto player = manager->scene.get<PlayerComponent>(playerId);

	std::string hp = std::to_string(hpcomp->health);
	hp.resize(hp.size() - 5);
	std::string maxhp = std::to_string(hpcomp->maxHealth);
	maxhp.resize(maxhp.size() - 5);
	std::string hpstr = "HP: " + hp + "/" + maxhp;
	healthNum->setText(std::wstring(hpstr.begin(), hpstr.end()).c_str());

	rect<s32> screenrect = player->rootHUD->getRelativePosition();
	dimension2du newsize;
	newsize.set((u32)hpcomp->health / hpcomp->maxHealth * 192, 64);
	health->setRelativePosition(rect<s32>(position2di(0, screenrect.getHeight() - 64), newsize));
	healthNum->setRelativePosition(rect<s32>(position2di(0, screenrect.getHeight() - 96), dimension2du(300, 24)));
}
