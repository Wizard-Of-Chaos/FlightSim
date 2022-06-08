#include "HUDVelocityBar.h"
#include "GameController.h"
#include "GameStateController.h"

#include <iostream>

HUDVelocityBar::HUDVelocityBar(IGUIElement* root) : HUDElement(root)
{
	rect<s32> screen = root->getRelativePosition();

	type = HUD_ELEM_TYPE::VELOCITY_BAR;
	velocity = guienv->addImage(stateController->assets.getHUDAsset("velocitybar"), position2di(screen.getWidth() - 192, screen.getHeight() - 64), root);
	position = guienv->addStaticText(L"", rect<s32>(position2di(screen.getWidth() - (screen.getWidth()/2)-150, screen.getHeight() - 32), dimension2du(280, 32)), false, true, root);
	velocityNum = guienv->addStaticText(L"", rect<s32>(position2di(screen.getWidth() - 190, screen.getHeight() - 96), dimension2du(192, 32)), false, true, root);
	rotationSpeed = guienv->addStaticText(L"", rect<s32>(position2di(screen.getWidth() - 190, screen.getHeight() - 128), dimension2du(192, 32)), false, true, root);
	overrideStatus = guienv->addStaticText(L"", rect<s32>(position2di(screen.getWidth() - 190, screen.getHeight() - 160), dimension2du(192, 32)), false, true, root);
	velocity->setScaleImage(false);
	velocity->setVisible(true);
	velocityNum->setVisible(true);
	velocityNum->setOverrideColor(SColor(255, 200, 200, 200));
	IGUIFont* fnt = stateController->assets.getFontAsset("HUDFont");
	velocityNum->setOverrideFont(fnt);
	position->setVisible(true);
	position->setOverrideColor(SColor(255, 200, 200, 200));
	position->setOverrideFont(fnt);
	rotationSpeed->setVisible(true);
	rotationSpeed->setOverrideColor(SColor(255, 200, 200, 200));
	rotationSpeed->setOverrideFont(fnt);
	overrideStatus->setVisible(true);
	overrideStatus->setOverrideColor(SColor(255, 200, 200, 200));
	overrideStatus->setOverrideFont(fnt);
}

HUDVelocityBar::~HUDVelocityBar()
{
	velocity->remove();
	velocityNum->remove();
	position->remove();
	rotationSpeed->remove();
	overrideStatus->remove();
}

void HUDVelocityBar::updateElement(flecs::entity playerId)
{
	auto irr = playerId.get<IrrlichtComponent>();
	auto ship = playerId.get<ShipComponent>();
	auto rbc = playerId.get<BulletRigidBodyComponent>();
	auto player = playerId.get<PlayerComponent>();

	btVector3 pos = rbc->rigidBody.getCenterOfMassPosition();
	btScalar velLen = rbc->rigidBody.getLinearVelocity().length();
	btScalar angLen = rbc->rigidBody.getAngularVelocity().length();

	std::string x, y, z;
	x = fprecis(pos.x(), 5);
	y = fprecis(pos.y(), 5);
	z = fprecis(pos.z(), 5);

	std::string velnum = fprecis(velLen, 5);
	std::string angnum = fprecis(angLen * RADTODEG, 5);
	std::string overtoggle = (ship->safetyOverride) ? "OFF" : "ON";

	std::string vel = "Velocity: " + velnum;
	std::string postext = "Position: " + x + ", " + y + ", " + z;
	std::string ang = "Angular Velocity: " + angnum;
	std::string over = "Safety Override: " + overtoggle;

	velocityNum->setText(std::wstring(vel.begin(), vel.end()).c_str());
	position->setText(std::wstring(postext.begin(), postext.end()).c_str());
	rotationSpeed->setText(std::wstring(ang.begin(), ang.end()).c_str());
	overrideStatus->setText(std::wstring(over.begin(), over.end()).c_str());

	f32 maxvel = ship->linearMaxVelocity;
	rect<s32> screenrect = player->rootHUD->getRelativePosition();
	dimension2du newsize; 
	u32 width = (u32)((velLen / maxvel) * 192);
	width = (width >= 192) ? 192 : width;
	newsize.set(width, 64);
	velocity->setRelativePosition(rect<s32>(position2di(screenrect.getWidth()-width, screenrect.getHeight() - 64), newsize));
	velocityNum->setRelativePosition(rect<s32>(position2di(screenrect.getWidth() - 190, screenrect.getHeight() - 96), dimension2du(192, 32)));
	position->setRelativePosition(rect<s32>(position2di((screenrect.getWidth() / 2) - 150, screenrect.getHeight() - 32), dimension2du(280, 32)));
	rotationSpeed->setRelativePosition(rect<s32>(position2di(screenrect.getWidth() - 190, screenrect.getHeight() - 128), dimension2du(192, 32)));
	overrideStatus->setRelativePosition(rect<s32>(position2di(screenrect.getWidth() - 190, screenrect.getHeight() - 160), dimension2du(192, 32)));
}