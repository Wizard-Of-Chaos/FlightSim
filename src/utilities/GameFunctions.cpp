#include "GameFunctions.h"
#include "SceneManager.h"
#include "GameController.h"
#include "GameStateController.h"
#include "IrrlichtUtils.h"
#include <iostream>

//Sets the defaults in the scene manager for ship meshes.
void setDefaults(SceneManager* manager)
{
	ISceneManager* smgr = manager->controller->smgr;
	IVideoDriver* driver = manager->controller->driver;
	ISoundEngine* sndeng = manager->controller->soundEngine;

	manager->defaults.defaultShipMesh = smgr->getMesh("models/tux/Tuxedo.obj");
	manager->defaults.defaultShipTexture = driver->getTexture("models/tux/tuxedotex.png");
	manager->defaults.defaultWeaponMesh = smgr->getMesh("models/wazer/wazer.obj");
	manager->defaults.defaultObstacleMesh = smgr->getMesh("models/asteroid/Asteroid.obj");
	manager->defaults.defaultObstacleTexture = driver->getTexture("models/asteroid/Asteroid.jpg");
	if (!loadHull("attributes/hulls/Asteroid.bullet", manager->defaults.defaultObstacleHull)) {
		manager->defaults.defaultObstacleHull = createCollisionShapeFromMesh(manager->defaults.defaultObstacleMesh);
		saveHull("attributes/hulls/Asteroid.bullet", manager->defaults.defaultObstacleHull);
	}
	manager->defaults.defaultMissileMesh = smgr->getMesh("models/basicmissile/Missile.obj");

	manager->defaults.defaultProjectileTexture = driver->getTexture("effects/particlered.bmp");
	manager->defaults.defaultCrosshairTexture = driver->getTexture("hud/crosshair.png");
	manager->defaults.defaultSelectionTexture = driver->getTexture("hud/selection.png");
	manager->defaults.defaultContactTexture = driver->getTexture("hud/contact.png");
	manager->defaults.defaultContactMarkerTexture = driver->getTexture("hud/contactmarker.png");
	manager->defaults.defaultHealthBarTexture = driver->getTexture("hud/hp.png");
	manager->defaults.defaultVelocityBarTexture = driver->getTexture("hud/speed.png");
	manager->defaults.defaultFuelBarTexture = driver->getTexture("hud/fuel.png");

	manager->defaults.defaultJetTexture = driver->getTexture("effects/smokejet.png");
	manager->defaults.defaultEngineJetTexture = driver->getTexture("effects/tuxengine.png");
	manager->defaults.defaultExplosion = driver->getTexture("effects/kaboom.png");

	manager->defaults.defaultJetSoundLoop = sndeng->getSoundSource("audio/jetthrust.ogg");
	manager->defaults.defaultMusic = sndeng->getSoundSource("audio/music/space_boogaloo.ogg");
	manager->defaults.defaultEngineSoundLoop = sndeng->getSoundSource("audio/engineloop.ogg");
	manager->defaults.defaultLaserSound = sndeng->getSoundSource("audio/laser_shoot.ogg");
	manager->defaults.bonk = sndeng->getSoundSource("audio/bonk.ogg");
	manager->defaults.bonk->setDefaultVolume(10.f);
	manager->defaults.bonk->setDefaultMinDistance(200.f);
	manager->defaults.defaultLaserSound->setDefaultVolume(1.f);
	manager->defaults.defaultLaserSound->setDefaultMinDistance(2.f);

	manager->defaults.defaultGunSound = sndeng->getSoundSource("audio/pew.ogg");
	manager->defaults.defaultGunSound->setDefaultVolume(1.f);
	manager->defaults.defaultGunSound->setDefaultMinDistance(3.f);

	manager->defaults.defaultCloudTexture = driver->getTexture("effects/cloud.png");

	manager->defaults.defaultHUDFont = manager->controller->guienv->getFont("fonts/AgencyFB14px/AgencyFB14px.xml");
}


//Convenience functions to swap back and forth between irrlicht and bullet vectors.
vector3df btVecToIrr(btVector3 vec)
{
	return vector3df(vec.x(), vec.y(), vec.z());
}

btVector3 irrVecToBt(vector3df vec)
{
	return btVector3(vec.X, vec.Y, vec.Z);
}

//Convenience functions that create a (sorta) random vector in Irrlicht space.
vector3df randomVector()
{
	f32 x = static_cast<f32>(rand() / static_cast <f32>(900));
	f32 y = static_cast<f32>(rand() / static_cast <f32>(900));
	f32 z = static_cast<f32>(rand() / static_cast <f32>(900));

	return vector3df(x, y, z);
}

vector3df randomRotationVector()
{
	f32 x = static_cast<f32>(rand() / static_cast <f32>(360)) - 180.f;
	f32 y = static_cast<f32>(rand() / static_cast <f32>(360)) - 180.f;
	f32 z = static_cast<f32>(rand() / static_cast <f32>(360)) - 180.f;

	return vector3df(x, y, z);
}

vector3df getPointInSphere(vector3df center, f32 radius)
{
	f32 dist = 1.5f; // init to greater than 1
	f32 x, y, z;
	while (dist > 1.f) {
		f32 low = -1.f;
		f32 high = 1.f;
		x = low + static_cast<f32>(rand()) / (static_cast<f32>(RAND_MAX / (high - low)));
		y = low + static_cast<f32>(rand()) / (static_cast<f32>(RAND_MAX / (high - low)));
		z = low + static_cast<f32>(rand()) / (static_cast<f32>(RAND_MAX / (high - low)));
		dist = (x * x) + (y* y) + (z* z);
	}
	vector3df pos(x * radius, y * radius, z * radius);
	return center + pos;
}

bool isPointInSphere(vector3df& point, vector3df center, f32 radius)
{
	return (pow(point.X - center.X, 2.f) + pow(point.Y - center.Y, 2.f) + pow(point.Z - center.Z, 2.f) < pow(radius, 2));
}

void destroyObject(SceneManager* manager, EntityId id)
{
	if (!manager->scene.entityInUse(id)) return;
	auto irrComp = manager->scene.get<IrrlichtComponent>(id);
	auto rbc = manager->scene.get<BulletRigidBodyComponent>(id);

	auto ship = manager->scene.get<ShipComponent>(id);
	if (ship) {
		for (unsigned int i = 0; i < ship->hardpointCount; ++i) {
			destroyObject(manager, ship->weapons[i]);
		}
	}

	if (irrComp) {
		irrComp->node->removeAll();
		irrComp->node->remove();
	}
	if (rbc) {
		manager->bulletWorld->removeRigidBody(&rbc->rigidBody);
	}
	auto ghost = manager->scene.get<BulletGhostComponent>(id);
	if (ghost) {
		manager->bulletWorld->removeCollisionObject(&ghost->ghost);
	}

	manager->scene.destroyEntity(id);
}

bool initializeDefaultPlayer(SceneManager* manager, EntityId shipId)
{
	Scene* scene = &manager->scene;
	ISceneManager* smgr = manager->controller->smgr;

	auto shipIrr = scene->get<IrrlichtComponent>(shipId);
	if (!shipIrr) return false;
	ISceneNode* target = smgr->addEmptySceneNode(0);
	target->setPosition(shipIrr->node->getPosition());
	ICameraSceneNode* camera = smgr->addCameraSceneNode(target, vector3df(0, 5, -20), shipIrr->node->getPosition(), ID_IsNotSelectable, true);
	scene->assign<InputComponent>(shipId);
	auto player = scene->assign<PlayerComponent>(shipId);
	player->camera = camera;
	player->target = target;

	player->thrust = vector3df(0, 0, 0);
	player->rotation = vector3df(0, 0, 0);

	return true;
}

void initializeHealth(SceneManager* manager, EntityId id, f32 healthpool)
{
	auto hp = manager->scene.assign<HealthComponent>(id);
	auto dmg = manager->scene.assign<DamageTrackingComponent>(id);
	hp->health = healthpool;
	hp->maxHealth = healthpool;
}
void initializeDefaultHealth(SceneManager* manager, EntityId objectId)
{
	initializeHealth(manager, objectId, DEFAULT_MAX_HEALTH);
}

bool initializeDefaultHUD(SceneManager* manager, EntityId playerId)
{
	Scene* scene = &manager->scene;
	auto player = scene->get<PlayerComponent>(playerId);
	if (!player) return false;

	dimension2du baseSize = dimension2du(960, 540);
	player->rootHUD = manager->controller->guienv->addStaticText(L"", rect<s32>(position2di(0, 0), baseSize));

	//HUDElement* crossHUD = new HUDCrosshair(manager, player->rootHUD);
	HUDElement* selectHUD = new HUDActiveSelection(manager, player->rootHUD);
	HUDElement* healthHUD = new HUDHealthBar(manager, player->rootHUD);
	HUDElement* speedHUD = new HUDVelocityBar(manager, player->rootHUD);
	//player->HUD.push_back(crossHUD);
	player->HUD.push_back(selectHUD);
	player->HUD.push_back(healthHUD);
	player->HUD.push_back(speedHUD);
	return true;
}

EntityId explode(SceneManager* manager, vector3df position, f32 duration, f32 scale, f32 radius, f32 damage, f32 force)
{
	EntityId id = manager->scene.newEntity();
	auto exp = manager->scene.assign<ExplosionComponent>(id);
	exp->duration = duration;
	exp->lifetime = 0;
	exp->explosion = manager->controller->smgr->addParticleSystemSceneNode(true, 0, ID_IsNotSelectable, position);
	f32 scalefac = scale;
	auto em = exp->explosion->createPointEmitter(vector3df(0.04f * scalefac, 0.f, 0.f), 200, 500, SColor(0, 255, 255, 255), SColor(0, 255, 255, 255),
		50, 1000, 360, dimension2df(1.f, 1.f), dimension2df(5.f * scalefac, 5.f * scalefac));

	exp->explosion->setEmitter(em);
	em->drop();
	IParticleAffector* paf = exp->explosion->createFadeOutParticleAffector(SColor(0,0,0,0),100);
	exp->explosion->addAffector(paf);
	paf->drop();
	exp->explosion->setMaterialFlag(EMF_LIGHTING, false);
	exp->explosion->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
	exp->explosion->setMaterialTexture(0, manager->defaults.defaultExplosion);
	exp->explosion->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);

	exp->light = manager->controller->smgr->addLightSceneNode(0, position, SColorf(1.f, .9f, .1f));

	exp->force = force;
	exp->damage = damage;
	exp->radius = radius;

	return id;
}